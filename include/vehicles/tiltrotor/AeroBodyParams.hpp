// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_aero_body_params_hpp
#define msr_airlib_aero_body_params_hpp

#include "common/Common.hpp"
#include "RotorTiltableParams.hpp"
#include "AeroParams.hpp"
#include "sensors/SensorCollection.hpp"
#include "sensors/SensorFactory.hpp"
#include "vehicles/tiltrotor/api/TiltrotorApiBase.hpp"

namespace msr { namespace airlib {

class AeroBodyParams {
public: //types
    struct RotorTiltableConfiguration {
        //all fields are wrt center of mass in the body frame
        Vector3r position;
        Vector3r normal_nominal;
        Vector3r rotation_axis;
        bool is_fixed;
        real_T max_angle;
        RotorTurningDirection direction;
        RotorTiltableParams params; //each rotor may have different parameters if different motor types are used
                                    //on different parts of the aircraft

        RotorTiltableConfiguration()
        {}
        RotorTiltableConfiguration(const Vector3r& position_val, const Vector3r& normal_nominal_val, const Vector3r& rotation_axis_val,
            const bool is_fixed_val, const real_T max_angle_val, const RotorTurningDirection direction_val, const RotorTiltableParams& params_val)
            : position{position_val}, normal_nominal{normal_nominal_val}, rotation_axis{rotation_axis_val},
                is_fixed{is_fixed_val}, max_angle{max_angle_val}, direction{direction_val}, params{params_val}
        {}
    };

    struct Params {
        uint rotor_count;
        vector<RotorTiltableConfiguration> rotor_configs;
        real_T mass;
        Matrix3x3r inertia;

        AeroControlType aero_control_type;
        AeroParams aero_params;

        real_T restitution = 0.55f; // value of 1 would result in perfectly elastic collisions, 0 would be completely inelastic.
        real_T friction = 0.5f;
    };

protected: //must override by derived class
    virtual void setupParams() = 0;
    virtual const SensorFactory* getSensorFactory() const = 0;

public: //interface
    virtual std::unique_ptr<TiltrotorApiBase> createTiltrotorApi() = 0;

    virtual ~AeroBodyParams() = default;
    virtual void initialize(const AirSimSettings::VehicleSetting* vehicle_setting)
    {
        sensor_storage_.clear();
        sensors_.clear();

        setupParams();

        addSensorsFromSettings(vehicle_setting);
    }

    const Params& getParams() const
    {
        return params_;
    }
    Params& getParams()
    {
        return params_;
    }
    SensorCollection& getSensors()
    {
        return sensors_;
    }
    const SensorCollection& getSensors() const
    {
        return sensors_;
    }

    void addSensorsFromSettings(const AirSimSettings::VehicleSetting* vehicle_setting)
    {
        // use sensors from vehicle settings; if empty list, use default sensors.
        // note that the vehicle settings completely override the default sensor "list";
        // there is no piecemeal add/remove/update per sensor.
        const std::map<std::string, std::unique_ptr<AirSimSettings::SensorSetting>>& sensor_settings
            = vehicle_setting->sensors.size() > 0 ? vehicle_setting->sensors : AirSimSettings::AirSimSettings::singleton().sensor_defaults;

        getSensorFactory()->createSensorsFromSettings(sensor_settings, sensors_, sensor_storage_);
    }

protected:
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
        fl_params.rotor_params.calculateMaxThrust();
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
        fr_params.rotor_params.calculateMaxThrust();
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
        r_params.rotor_params.calculateMaxThrust();
        RotorTiltableConfiguration r_config(r_pos, r_norm, r_rot, r_fixed, r_max, r_direction, r_params);
        params.rotor_configs.push_back(r_config);

        params.mass = 1.f;
        params.inertia << 0.0165, 0.f, 0.000048,
                          0.f, 0.025, 0.f,
                          0.000048, 0.f, 0.0282;

        params.aero_control_type = AeroControlType::ElevonRudder;
        params.aero_params = AeroParams();
    }

private: //fields
    Params params_;
    SensorCollection sensors_; //maintains sensor type indexed collection of sensors
    vector<unique_ptr<SensorBase>> sensor_storage_; //RAII for created sensors
};

}} //namespace

#endif