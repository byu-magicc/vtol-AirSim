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

private: //fields
    Params params_;
    SensorCollection sensors_; //maintains sensor type indexed collection of sensors
    vector<unique_ptr<SensorBase>> sensor_storage_; //RAII for created sensors
};

}} //namespace

#endif