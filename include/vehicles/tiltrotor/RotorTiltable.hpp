// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.



#ifndef airsimcore_RotorTiltable_hpp
#define airsimcore_RotorTiltable_hpp

#include <limits>
#include "common/Common.hpp"
#include "physics/Environment.hpp"
#include "common/FirstOrderFilter.hpp"
#include "physics/PhysicsBodyVertex.hpp"
#include "multirotor/Rotor.hpp"
#include "RotorTiltableParams.hpp"

namespace msr { namespace airlib {

//RotorTiltable inherits from Rotor. It is a rotor that is allowed to rotate
//about a single axis with specified range. 

class RotorTiltable : public Rotor {
public: //types
    struct TiltOutput {
        Output rotor_output;
        real_T angle_signal_filtered;
        real_T angle_signal_input;
        real_T angle;
    };

public: //methods
    RotorTiltable()
    {
        //allow default constructor with later call for initialize
    }
    RotorTiltable(const Vector3r& position, const Vector3r& normal_nominal,
        RotorTurningDirection turning_direction, const Vector3r& rotation_axis,
        const real_T max_angle, const RotorTiltableParams& params, const Environment* environment, uint id = -1)
    {
        initialize(position, normal_nominal, turning_direction, rotation_axis, max_angle, params, environment, id);
    }

    void initialize(const Vector3r& position, const Vector3r& normal_nominal,
        RotorTurningDirection turning_direction, const Vector3r& rotation_axis,
        const real_T max_angle, const RotorTiltableParams& params, const Environment* environment, uint id = -1)
    {
        normal_nominal_ = normal_nominal;
        rotation_axis_ = rotation_axis;
        max_angle_ = max_angle;
        tilt_params_ = params;

        Rotor::initialize(position, normal_nominal, turning_direction, params.rotor_params, environment, id);
    }

    //-1 to 1, will be scaled to -max_angle_, +max_angle_
    void setAngleSignal(real_T angle_signal)
    {
        angle_signal_filter_.setInput(Utils::clip(angle_signal, -1.0f, 1.0f));
    }

    //*** Start: UpdatableState implementation ***//
    virtual void resetImplementation() override
    {
        Rotor::resetImplementation();

        //update environmental factors before we call base
        updateEnvironmentalFactors();

        angle_signal_filter_.reset();
        angle_filter_.reset();
        normal_current_ = normal_nominal_;

        // setOutput(output_, params_, control_signal_filter_, turning_direction_);
    }

    virtual void update() override
    {
        //update environmental factors before we call base
        updateEnvironmentalFactors();

        //this will in turn call setWrench
        PhysicsBodyVertex::update();

        //update our state
        setOutput(output_, params_, control_signal_filter_, turning_direction_);

        //update filter - this should be after so that first output is same as initial
        control_signal_filter_.update();
    }

    virtual void reportState(StateReporter& reporter) override
    {
        reporter.writeValue("Dir", static_cast<int>(turning_direction_));
        reporter.writeValue("Ctrl-in", output_.control_signal_input);
        reporter.writeValue("Ctrl-fl", output_.control_signal_filtered);
        reporter.writeValue("speed", output_.speed);
        reporter.writeValue("thrust", output_.thrust);
        reporter.writeValue("torque", output_.torque_scaler);
    }
    //*** End: UpdatableState implementation ***//



protected:


private: //methods



private: //fields

    Vector3r normal_nominal_;
    Vecotr3r normal_current_;
    Vector3r rotation_axis_;
    real_t max_angle_;
    RotorTiltableParams tilt_params_;
    FirstOrderFilter<real_T> angle_signal_filter_; //first order filter for rotor angle signal
    FirstOrderFilter<real_T> angle_filter_; //first order filter for actual rotor angle (will be much slower than angle_signal_filter)

};


}} //namespace
#endif
