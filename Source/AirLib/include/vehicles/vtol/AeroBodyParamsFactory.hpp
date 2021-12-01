// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_vehicles_AeroBodyParamsFactory_hpp
#define msr_airlib_vehicles_AeroBodyParamsFactory_hpp

#include "vehicles/vtol/firmwares/vtol_simple/VtolSimpleParams.hpp"
#include "vehicles/vtol/firmwares/mavlink/Px4VtolParams.hpp"

namespace msr
{
namespace airlib
{

    class AeroBodyParamsFactory
    {
    public:
        static std::unique_ptr<AeroBodyParams> createConfig(const AirSimSettings::VehicleSetting* vehicle_setting,
                                                            std::shared_ptr<const SensorFactory> sensor_factory)
        {
            std::unique_ptr<AeroBodyParams> config;

            if (vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypePX4Vtol) {
                config.reset(new Px4VtolParams(*static_cast<const AirSimSettings::MavLinkVehicleSetting*>(vehicle_setting),
                                                    sensor_factory));
            }
            else if (vehicle_setting->vehicle_type == "" || //default config
                     vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypeVtolSimple) {
                config.reset(new VtolSimpleParams(vehicle_setting, sensor_factory));
            }
            else
                throw std::runtime_error(Utils::stringf(
                    "Cannot create vehicle config because vehicle name '%s' is not recognized",
                    vehicle_setting->vehicle_name.c_str()));

            config->initialize(vehicle_setting);

            return config;
        }
    };
}
} //namespace
#endif
