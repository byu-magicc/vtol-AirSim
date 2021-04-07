// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.



#ifndef airsimcore_RotorTiltable_hpp
#define airsimcore_RotorTiltable_hpp

#include <limits>
#include "common/Common.hpp"
#include "physics/Environment.hpp"
#include "common/FirstOrderFilter.hpp"
#include "physics/PhysicsBodyVertex.hpp"
#include "vehicles/multirotor/RotorActuator.hpp"
#include "RotorTiltableParams.hpp"

namespace msr { namespace airlib {

//RotorTiltable inherits from RotorActuator. It is a rotor that is allowed to rotate
//about a single axis with specified range.

class RotorTiltable : public RotorActuator {
public: //types
    struct TiltOutput {
        Output rotor_output;
        real_T angle_signal_filtered;
        real_T angle_signal_input;
        real_T angle;
        real_T angle_from_vertical; //only used for visualization
        bool is_fixed;
    };

public: //methods
    RotorTiltable()
    {
        //allow default constructor with later call for initialize
    }
    RotorTiltable(const Vector3r& position, const Vector3r& normal_nominal,
        RotorTurningDirection turning_direction, bool is_fixed, const Vector3r& rotation_axis,
        const real_T max_angle, const RotorTiltableParams& params, const Environment* environment, uint id = -1)
    {
        initialize(position, normal_nominal, turning_direction, is_fixed, rotation_axis, max_angle, params, environment, id);
    }

    void initialize(const Vector3r& position, const Vector3r& normal_nominal,
        RotorTurningDirection turning_direction, bool is_fixed, const Vector3r& rotation_axis,
        const real_T max_angle, const RotorTiltableParams& params, const Environment* environment, uint id = -1)
    {
        normal_nominal_ = normal_nominal;
        normal_nominal_.normalize();
        is_fixed_ = is_fixed; //don't allow rotor to rotate if is_fixed_ is true
        rotation_axis_ = rotation_axis; //this gets ignored if is_fixed_ is true
        rotation_axis_.normalize();
        max_angle_ = max_angle;
        tilt_params_ = params;
        environment_ = environment;

        angle_signal_filter_.initialize(params.angle_signal_filter_tc, 0, 0);
        angle_filter_.initialize(params.angle_filter_tc, 0, 0);
        airspeed_body_vector_ = Vector3r::Zero();

        RotorActuator::initialize(position, normal_nominal, turning_direction, params.rotor_params, environment, id);
    }

    //-1 to 1, will be scaled to -max_angle_, +max_angle_
    void setAngleSignal(real_T angle_signal)
    {
        if(!is_fixed_)
            angle_signal_filter_.setInput(Utils::clip(angle_signal, -1.0f, 1.0f));
    }

    void setAirspeedRotor(const Vector3r& airspeed_body_vector)
    {
        airspeed_body_vector_ = airspeed_body_vector;
    }

    TiltOutput getOutput() const
    {
        return tilt_output_;
    }

    //*** Start: UpdatableState implementation ***//
    virtual void resetImplementation() override
    {
        //update environmental factors before we call base
        updateEnvironmentalFactors();

        RotorActuator::resetImplementation();

        angle_signal_filter_.reset();
        angle_filter_.reset();
        normal_current_ = normal_nominal_;
        setNormal(normal_nominal_);
        airspeed_body_vector_ = Vector3r::Zero();

        setTiltOutput(tilt_output_, tilt_params_, angle_signal_filter_, angle_filter_, is_fixed_);
    }

    virtual void update() override
    {
        //update environmental factors before we call base
        updateEnvironmentalFactors();

        //call rotor update
        RotorActuator::update();

        //update tilt output
        setTiltOutput(tilt_output_, tilt_params_, angle_signal_filter_, angle_filter_, is_fixed_);

        //update angle filters
        if(!is_fixed_) {
            angle_signal_filter_.update();
            angle_filter_.setInput(max_angle_ * angle_signal_filter_.getOutput());
            angle_filter_.update();

            //calculate new normal and set normal in PhysicsBodyVertex
            AngleAxisr rotate = AngleAxisr(angle_filter_.getOutput(), rotation_axis_);
            normal_current_ = rotate * normal_nominal_;
            setNormal(normal_current_);
        }
    }

    virtual void reportState(StateReporter& reporter) override
    {
        reporter.writeValue("Dir", static_cast<int>(tilt_output_.rotor_output.turning_direction));
        reporter.writeValue("Ctrl-in", tilt_output_.rotor_output.control_signal_input);
        reporter.writeValue("Ctrl-fl", tilt_output_.rotor_output.control_signal_filtered);
        reporter.writeValue("speed", tilt_output_.rotor_output.speed);
        reporter.writeValue("thrust", tilt_output_.rotor_output.thrust);
        reporter.writeValue("torque_scaler", tilt_output_.rotor_output.torque_scaler);
        reporter.writeValue("Angl-in", tilt_output_.angle_signal_input);
        reporter.writeValue("Angl-fl", tilt_output_.angle_signal_filtered);
        reporter.writeValue("Angle", tilt_output_.angle);
        reporter.writeValue("Fixed", static_cast<int>(tilt_output_.is_fixed));
    }
    //*** End: UpdatableState implementation ***//



protected:
    //override RotorActuator's setWrench function using TiltOutput
    virtual void setWrench(Wrench& wrench) override
    {
        if (tilt_params_.use_simple_rotor_model) {
            RotorActuator::setWrench(wrench);
        }
        else {
            Vector3r normal = getNormal();
            wrench.force = normal * tilt_output_.rotor_output.thrust;
            wrench.torque = normal * tilt_output_.rotor_output.torque_scaler;
        }
    }

private: //methods
    void setTiltOutput(TiltOutput& tilt_output, const RotorTiltableParams& params, const FirstOrderFilter<real_T>& angle_signal_filter, const FirstOrderFilter<real_T>& angle_filter, bool is_fixed)
    {
        //populate rotor_output with output from RotoActuatorr
        tilt_output.rotor_output = RotorActuator::getOutput();

        //if we want to use more complicated rotor model, need to modify thrust and torque outputs
        if(!params.use_simple_rotor_model)
            calculateThrustTorque(tilt_output.rotor_output.thrust, tilt_output.rotor_output.torque_scaler, tilt_output.rotor_output.control_signal_filtered,
                tilt_params_, tilt_output.rotor_output.turning_direction, normal_current_.dot(airspeed_body_vector_), air_density_);

        tilt_output.angle_signal_filtered = angle_signal_filter.getOutput();
        tilt_output.angle_signal_input = angle_signal_filter.getInput();
        tilt_output.angle = angle_filter.getOutput();
        Vector3r normal = getNormal();
        tilt_output.angle_from_vertical = VectorMath::sgn(normal(0)) * std::acos(-normal(2));
        tilt_output.is_fixed = is_fixed;
    }

    //this more complicated rotor model is necessary because it more acurately calculates the effects of airspeed on the thrust a rotor is able to produce
    //it wouldn't really matter for multirotors that never reach high airspeeds, but is very important for fixedwing vehicles that travel at high airspeeds
    static void calculateThrustTorque(real_T& thrust, real_T& torque_scalar, const real_T throttle, const RotorTiltableParams& params,
        const RotorTurningDirection turning_direction, const real_T airspeed, const real_T air_density)
    {
        //motor model from "Small Unmanned Aircraft: Theory and Practice", supplement
        //see https://uavbook.byu.edu/lib/exe/fetch.php?media=uavbook_supplement.pdf, pg. 8
        real_T v_in = params.max_voltage * throttle;
        real_T a = params.CQ0 * air_density * pow(params.prop_diameter, 5) / pow(2*M_PIf, 2);
        real_T b = airspeed * params.CQ1 * air_density * pow(params.prop_diameter, 4) / (2*M_PIf) + pow(params.motor_KQ, 2) / params.motor_resistance;
        real_T c = pow(airspeed, 2) * params.CQ2 * air_density * pow(params.prop_diameter, 3) - v_in * params.motor_KQ / params.motor_resistance + params.motor_KQ * params.no_load_current;
        real_T Omega_op = (-b + sqrt(pow(b, 2) - 4*a*c)) / (2 * a);
        real_T J_op = 2 * M_PIf * airspeed / (Omega_op * params.prop_diameter);
        real_T CT = params.CT2 * pow(J_op, 2) + params.CT1 * J_op + params.CT0;
        real_T CQ = params.CQ2 * pow(J_op, 2) + params.CQ1 * J_op + params.CQ0;
        real_T n = Omega_op / (2 * M_PIf);
        real_T T_p = air_density * pow(n, 2) * pow(params.prop_diameter, 4) * CT;
        real_T Q_p = air_density * pow(n, 2) * pow(params.prop_diameter, 5) * CQ;

        thrust = T_p;
        torque_scalar = Q_p * static_cast<int>(turning_direction);
    }

    void updateEnvironmentalFactors()
    {
        //update air density
        air_density_ = environment_->getState().air_density;
    }


private: //fields

    Vector3r normal_nominal_;
    Vector3r normal_current_;
    bool is_fixed_;
    Vector3r rotation_axis_;
    real_T max_angle_;
    RotorTiltableParams tilt_params_;
    FirstOrderFilter<real_T> angle_signal_filter_; //first order filter for rotor angle signal
    FirstOrderFilter<real_T> angle_filter_; //first order filter for actual rotor angle (will be much slower than angle_signal_filter)
    TiltOutput tilt_output_;
    Vector3r airspeed_body_vector_;

    const Environment* environment_ = nullptr;
    real_T air_density_;
};


}} //namespace
#endif
