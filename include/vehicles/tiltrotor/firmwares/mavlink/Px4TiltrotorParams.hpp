// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_vehicles_Px4TiltRotor_hpp
#define msr_airlib_vehicles_Px4TiltRotor_hpp

#include "vehicles/tiltrotor/firmwares/mavlink/MavLinkTiltrotorApi.hpp"
#include "common/AirSimSettings.hpp"
#include "sensors/SensorFactory.hpp"
#include "vehicles/tiltrotor/AeroBodyParams.hpp"

namespace msr { namespace airlib {

class Px4TiltrotorParams : public AeroBodyParams {
public:
    Px4TiltrotorParams(const AirSimSettings::MavLinkVehicleSetting& vehicle_setting, std::shared_ptr<const SensorFactory> sensor_factory)
        : sensor_factory_(sensor_factory)
    {
        connection_info_ = getConnectionInfo(vehicle_setting);
    }

    virtual ~Px4TiltrotorParams() = default;

    virtual std::unique_ptr<TiltrotorApiBase> createTiltrotorApi() override
    {
        unique_ptr<TiltrotorApiBase> api(new MavLinkTiltrotorApi());
        auto api_ptr = static_cast<MavLinkTiltrotorApi*>(api.get());
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
    //TODO: is there a way to read params in from vehicle_setting?
    void setupGenericFixedWing(Params& params)
    {
        params.rotor_count = 1;

        //front rotor
        Vector3r front_pos(1.f, 0.f, 0.f);
        Vector3r front_norm(1.f, 0.f, 0.f);
        Vector3r front_rot(1.f, 0.f, 0.f); //this gets ignored
        bool front_fixed = true;
        real_T front_max = 0.f;
        RotorTurningDirection front_direction = RotorTurningDirection::RotorTurningDirectionCCW;
        RotorTiltableParams front_params = RotorTiltableParams();
        RotorTiltableConfiguration front_config(front_pos, front_norm, front_rot, front_fixed, front_max, front_direction, front_params);
        params.rotor_configs.push_back(front_config);

        params.mass = 1.f;
        params.inertia << 0.0165, 0.f, 0.000048,
                          0.f, 0.025, 0.f,
                          0.000048, 0.f, 0.0282;

        params.aero_control_type = AeroControlType::ElevonRudder;
        params.aero_params = AeroParams();
    }

    void setupTriTiltrotor(Params& params)
    {
        //the rotor setup of the tritiltrotor is 
        //    (0)     (1)
        //      \     /
        //        \ /
        //         |
        //        (2)
        //where the front two rotors tilt and the rear is fixed in the negative z direction
        //These parameters are all for the Convergence from E-flite

        params.rotor_count = 3;
        
        //front left rotor
        Vector3r fl_pos(0.12, -0.2, 0.f);
        Vector3r fl_norm(sqrt(2.f)/2.f, 0.f, -sqrt(2.f)/2.f);
        Vector3r fl_rot(0.f, 1.f, 0.f);
        bool fl_fixed = false;
        real_T fl_max = 60.f * M_PIf / 180.f;
        RotorTurningDirection fl_direction = RotorTurningDirection::RotorTurningDirectionCCW;
        RotorTiltableParams fl_params = RotorTiltableParams();
        RotorTiltableConfiguration fl_config(fl_pos, fl_norm, fl_rot, fl_fixed, fl_max, fl_direction, fl_params);
        params.rotor_configs.push_back(fl_config);

        //front right rotor
        Vector3r fr_pos(0.12, 0.2, 0.f);
        Vector3r fr_norm(sqrt(2.f)/2.f, 0.f, -sqrt(2.f)/2.f);
        Vector3r fr_rot(0.f, 1.f, 0.f);
        bool fr_fixed = false;
        real_T fr_max = 60.f * M_PIf / 180.f;
        RotorTurningDirection fr_direction = RotorTurningDirection::RotorTurningDirectionCCW;
        RotorTiltableParams fr_params = RotorTiltableParams();
        RotorTiltableConfiguration fr_config(fr_pos, fr_norm, fr_rot, fr_fixed, fr_max, fr_direction, fr_params);
        params.rotor_configs.push_back(fr_config);

        //rear rotor
        Vector3r r_pos(-0.24, 0.f, 0.f);
        Vector3r r_norm(0.f, 0.f, -1.f);
        Vector3r r_rot(1.f, 0.f, 0.f); //this gets ignored
        bool r_fixed = true;
        real_T r_max = 0.f;
        RotorTurningDirection r_direction = RotorTurningDirection::RotorTurningDirectionCCW;
        RotorTiltableParams r_params = RotorTiltableParams();
        RotorTiltableConfiguration r_config(r_pos, r_norm, r_rot, r_fixed, r_max, r_direction, r_params);
        params.rotor_configs.push_back(r_config);

        params.mass = 1.f;
        params.inertia << 0.0165, 0.f, 0.000048,
                          0.f, 0.025, 0.f,
                          0.000048, 0.f, 0.0282;

        params.aero_control_type = AeroControlType::ElevonRudder;
        params.aero_params = AeroParams();
    }

    void setupQuadPlane(Params& params)
    {
        //TODO
        throw std::logic_error{"QuadPlane params not yet implemented."};
    }

    static const AirSimSettings::MavLinkConnectionInfo& getConnectionInfo(const AirSimSettings::MavLinkVehicleSetting& vehicle_setting)
    {
        return vehicle_setting.connection_info;
    }


private:
    AirSimSettings::MavLinkConnectionInfo connection_info_;
    std::shared_ptr<const SensorFactory> sensor_factory_;

};

}} //namespace
#endif
