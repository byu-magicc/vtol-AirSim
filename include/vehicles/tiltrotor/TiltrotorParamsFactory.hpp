// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_vehicles_TiltrotorParamsFactory_hpp
#define msr_airlib_vehicles_TiltrotorParamsFactory_hpp

#include "vehicles/tiltrotor/firmwares/mavlink/MavLinkTiltrotorApi.hpp"
#include "vehicles/tiltrotor/firmwares/mavlink/Px4TiltrotorParams.hpp"
#include "vehicles/tiltrotor/firmwares/simple_flight/SimpleFlightQuadXParams.hpp"
#include "vehicles/tiltrotor/firmwares/mavlink/ArduCopterSoloParams.hpp"
#include "vehicles/tiltrotor/firmwares/arducopter/ArduCopterParams.hpp"


namespace msr { namespace airlib {

class TiltrotorParamsFactory {
public:
    static std::unique_ptr<TiltrotorParams> createConfig(const AirSimSettings::VehicleSetting* vehicle_setting,
        std::shared_ptr<const SensorFactory> sensor_factory)
    {
        std::unique_ptr<TiltrotorParams> config;

        if (vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypePX4) {
            config.reset(new Px4TiltrotorParams(*static_cast<const AirSimSettings::MavLinkVehicleSetting*>(vehicle_setting),
                sensor_factory));
        }
        else if (vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypeArduCopterSolo) {
            config.reset(new ArduCopterSoloParams(*static_cast<const AirSimSettings::MavLinkVehicleSetting*>(vehicle_setting), sensor_factory));
        }
        else if (vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypeArduCopter) {
            config.reset(new ArduCopterParams(*static_cast<const AirSimSettings::MavLinkVehicleSetting*>(vehicle_setting), sensor_factory));
        }
        else if (vehicle_setting->vehicle_type == "" || //default config
            vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypeSimpleFlight) {
            config.reset(new SimpleFlightQuadXParams(vehicle_setting, sensor_factory));
        }
        else
            throw std::runtime_error(Utils::stringf(
                "Cannot create vehicle config because vehicle name '%s' is not recognized",
                vehicle_setting->vehicle_name.c_str()));

        config->initialize(vehicle_setting);

        return config;
    }
};

}} //namespace
#endif
