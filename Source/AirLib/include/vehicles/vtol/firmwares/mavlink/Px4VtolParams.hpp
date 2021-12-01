// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_vehicles_Px4TiltRotor_hpp
#define msr_airlib_vehicles_Px4TiltRotor_hpp

#include "vehicles/vtol/firmwares/mavlink/MavLinkVtolApi.hpp"
#include "common/AirSimSettings.hpp"
#include "sensors/SensorFactory.hpp"
#include "vehicles/vtol/AeroBodyParams.hpp"

namespace msr
{
namespace airlib
{

    class Px4VtolParams : public AeroBodyParams
    {
    public:
        Px4VtolParams(const AirSimSettings::MavLinkVehicleSetting& vehicle_setting, std::shared_ptr<const SensorFactory> sensor_factory)
            : sensor_factory_(sensor_factory)
        {
            connection_info_ = getConnectionInfo(vehicle_setting);
        }

        virtual ~Px4VtolParams() = default;

        virtual std::unique_ptr<VtolApiBase> createVtolApi() override
        {
            unique_ptr<VtolApiBase> api(new MavLinkVtolApi());
            auto api_ptr = static_cast<MavLinkVtolApi*>(api.get());
            api_ptr->initialize(connection_info_, &getSensors(), true);

            return api;
        }

        virtual void setupParams() override
        {
            auto& params = getParams();

            if (connection_info_.model == "TriTiltrotor") {
                setupTriTiltrotor(params);
            }
            else if (connection_info_.model == "QuadPlane") {
                setupQuadPlane(params);
            }
            else //Generic
                setupGenericFixedWing(params);
        }

    protected:
        virtual const SensorFactory* getSensorFactory() const override
        {
            return sensor_factory_.get();
        }

    private:
        void setupQuadPlane(Params& params)
        {
            //TODO
            throw std::logic_error{ "QuadPlane params not yet implemented." };
        }

        static const AirSimSettings::MavLinkConnectionInfo& getConnectionInfo(const AirSimSettings::MavLinkVehicleSetting& vehicle_setting)
        {
            return vehicle_setting.connection_info;
        }

    private:
        AirSimSettings::MavLinkConnectionInfo connection_info_;
        std::shared_ptr<const SensorFactory> sensor_factory_;
    };
}
} //namespace
#endif
