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

namespace msr
{
namespace airlib
{

    class AeroBodyParams
    {
        //All units are SI
    public: //types
        struct RotorTiltableConfiguration
        {
            Vector3r position; //relative to center of gravity of vehicle body
            Vector3r normal_nominal;
            Vector3r rotation_axis;
            bool is_fixed;
            real_T max_angle;
            RotorTurningDirection direction;
            RotorTiltableParams params; //each rotor may have different parameters if different motor types are used on different parts of the aircraft

            RotorTiltableConfiguration()
            {
            }
            RotorTiltableConfiguration(const Vector3r& position_val, const Vector3r& normal_nominal_val, const Vector3r& rotation_axis_val,
                                       const bool is_fixed_val, const real_T max_angle_val, const RotorTurningDirection direction_val, const RotorTiltableParams& params_val)
                : position{ position_val }, normal_nominal{ normal_nominal_val }, rotation_axis{ rotation_axis_val }, is_fixed{ is_fixed_val }, max_angle{ max_angle_val }, direction{ direction_val }, params{ params_val }
            {
            }
        };

        struct Params
        {
            uint rotor_count;
            vector<RotorTiltableConfiguration> rotor_configs;
            real_T mass;
            Matrix3x3r inertia;

            real_T restitution = 0.55f; // value of 1 would result in perfectly elastic collisions, 0 would be completely inelastic.
            real_T friction = 0.5f;
            AeroParams aero_params;
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
            const auto& sensor_settings = vehicle_setting->sensors;

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

            params.aero_params = AeroParams();
        }

        void setupQuadPlane(Params& params)
        {
            // rotor setup of a quadplane is
            //    (1)       (0)
            //     |         |
            //     |---------|
            //     |   (4)   |
            //    (3)       (2)
            // where rotors 0-3 are fixed in the negative z direction and 
            // rotor 4 is fixed in the positive x direction

            params.rotor_count = 5;

            // Lift rotors
            RotorTiltableParams lift_rotor_params = RotorTiltableParams();
            lift_rotor_params.rotor_params.calculateMaxThrust();
            lift_rotor_params.max_voltage = 24.8f;
            lift_rotor_params.prop_diameter = 13.f * (0.0254f);
            lift_rotor_params.motor_resistance = 0.03;
            lift_rotor_params.motor_KV = 520;
            lift_rotor_params.motor_KQ = (1.0 / lift_rotor_params.motor_KV) * 60.0 / (2.0 * M_PIf);
            lift_rotor_params.no_load_current = 1.1;
            lift_rotor_params.CT0 = 0.10101;
            lift_rotor_params.CT1 = -0.05183;
            lift_rotor_params.CT2 = -0.13583;
            lift_rotor_params.CQ0 = 0.00772;
            lift_rotor_params.CQ1 = -0.00314;
            lift_rotor_params.CQ2 = -0.00926;

            // Rotor 0
            Vector3r r0_pos(.49f, .37f, 0.f);
            Vector3r r0_norm(0.f, 0.f, -1.f);
            Vector3r r0_rot(1.f, 0.f, 0.f); //this gets ignored
            bool r0_fixed = true;
            real_T r0_max = 0.f;
            RotorTurningDirection r0_direction = RotorTurningDirection::RotorTurningDirectionCCW;
            RotorTiltableParams r0_params = lift_rotor_params;
            RotorTiltableConfiguration r0_config(r0_pos, r0_norm, r0_rot, r0_fixed, r0_max, r0_direction, r0_params);
            params.rotor_configs.push_back(r0_config);

            // Rotor 1
            Vector3r r1_pos(.49f, -.37f, 0.f);
            Vector3r r1_norm(0.f, 0.f, -1.f);
            Vector3r r1_rot(1.f, 0.f, 0.f); //this gets ignored
            bool r1_fixed = true;
            real_T r1_max = 0.f;
            RotorTurningDirection r1_direction = RotorTurningDirection::RotorTurningDirectionCW;
            RotorTiltableParams r1_params = lift_rotor_params;
            RotorTiltableConfiguration r1_config(r1_pos, r1_norm, r1_rot, r1_fixed, r1_max, r1_direction, r1_params);
            params.rotor_configs.push_back(r1_config);

            // Rotor 2
            Vector3r r2_pos(-.45f, .37f, 0.f);
            Vector3r r2_norm(0.f, 0.f, -1.f);
            Vector3r r2_rot(1.f, 0.f, 0.f); //this gets ignored
            bool r2_fixed = true;
            real_T r2_max = 0.f;
            RotorTurningDirection r2_direction = RotorTurningDirection::RotorTurningDirectionCW;
            RotorTiltableParams r2_params = lift_rotor_params;
            RotorTiltableConfiguration r2_config(r2_pos, r2_norm, r2_rot, r2_fixed, r2_max, r2_direction, r2_params);
            params.rotor_configs.push_back(r2_config);

            // Rotor 3
            Vector3r r3_pos(-.45f, -.37f, 0.f);
            Vector3r r3_norm(0.f, 0.f, -1.f);
            Vector3r r3_rot(1.f, 0.f, 0.f); //this gets ignored
            bool r3_fixed = true;
            real_T r3_max = 0.f;
            RotorTurningDirection r3_direction = RotorTurningDirection::RotorTurningDirectionCCW;
            RotorTiltableParams r3_params = lift_rotor_params;
            RotorTiltableConfiguration r3_config(r3_pos, r3_norm, r3_rot, r3_fixed, r3_max, r3_direction, r3_params);
            params.rotor_configs.push_back(r3_config);

            // Rotor 4
            Vector3r r4_pos(-.41f, 0.f, 0.f);
            Vector3r r4_norm(1.f, 0.f, 0.f);
            Vector3r r4_rot(1.f, 0.f, 0.f); //this gets ignored
            bool r4_fixed = true;
            real_T r4_max = 0.f;
            RotorTurningDirection r4_direction = RotorTurningDirection::RotorTurningDirectionCCW;
            RotorTiltableParams r4_params = RotorTiltableParams();
            r4_params.rotor_params.calculateMaxThrust();
            r4_params.max_voltage = 16.5;
            r4_params.prop_diameter = 13 * (0.0254);
            r4_params.motor_resistance = 0.013;
            r4_params.motor_KV = 740;
            r4_params.motor_KQ = (1.0 / r4_params.motor_KV) * 60.0 / (2.0 * M_PIf);
            r4_params.no_load_current = 1.72;
            r4_params.CT0 = 0.10101;
            r4_params.CT1 = -0.05183;
            r4_params.CT2 = -0.13583;
            r4_params.CQ0 = 0.00772;
            r4_params.CQ1 = -0.00314;
            r4_params.CQ2 = -0.00926;
            RotorTiltableConfiguration r4_config(r4_pos, r4_norm, r4_rot, r4_fixed, r4_max, r4_direction, r4_params);
            params.rotor_configs.push_back(r4_config);

            params.mass = 5.81f;
            params.inertia << 0.335f, 0.f, -0.029f,
                0.f, 0.140f, 0.f,
                -0.029f, 0.f, 0.40f;

            params.aero_params = AeroParams();
            params.aero_params.flap_max_angle = 32 * M_PIf / 180;
            params.aero_params.S = .96;
            params.aero_params.b = 2.5;
            params.aero_params.c = .378;
            params.aero_params.aspect_ratio = pow(params.aero_params.b, 2) / params.aero_params.S;

            params.aero_params.CL.setCoefficients(0.0477, 4.06, 0.0, 0.0, 3.87, 0.0, 0.0, 0.7, 0.0);
            params.aero_params.CD.setCoefficients(0.0107, 0.0791, .115, 0.0, 0.0, 0.0, 0.0, 0.0196, 0.0);
            params.aero_params.CY.setCoefficients(1.08e-8, 0.0, -0.194, -0.137, 0.0, 0.0839, 0.0439, 0.0, 0.0);
            params.aero_params.Cl.setCoefficients(1.29e-7, 0.0, -0.0751, -0.404, 0.0, 0.0555, 0.202, 0.0, 0.0);
            params.aero_params.Cm.setCoefficients(0.00439, -0.227, 0.0, 0.0, -1.3, 0.0, 0.0, -0.325, 0.0);
            params.aero_params.Cn.setCoefficients(1.51e-7, 0.0, 0.0312, 0.00437, 0.0, -0.012, -0.00628, 0.0, 0.0);
            params.aero_params.aero_control_mixer = (Matrix3x3r() << -0.5f, 0.5f, 0.0f, //note that some signs are flipped because on actual aircraft,
                                                     0.5f,
                                                     0.5f,
                                                     0.0f, //+1 makes left elevon go down and +1 makes right elevon go up
                                                     0.0f,
                                                     0.0f,
                                                     0.0f)
                                                        .finished(); //convergence aircraft has no rudder
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
            real_T nom_ang_rad = 57.5f * M_PIf / 180.0f;
            Vector3r fl_norm(cos(nom_ang_rad), 0.f, -sin(nom_ang_rad));
            Vector3r fl_rot(0.f, -1.f, 0.f);
            bool fl_fixed = false;
            real_T fl_max = nom_ang_rad;
            RotorTurningDirection fl_direction = RotorTurningDirection::RotorTurningDirectionCW;
            RotorTiltableParams fl_params = RotorTiltableParams();
            fl_params.rotor_params.calculateMaxThrust();
            RotorTiltableConfiguration fl_config(fl_pos, fl_norm, fl_rot, fl_fixed, fl_max, fl_direction, fl_params);
            params.rotor_configs.push_back(fl_config);

            //front right rotor
            Vector3r fr_pos(0.12, 0.2, 0.f);
            Vector3r fr_norm(fl_norm);
            Vector3r fr_rot(0.f, 1.f, 0.f);
            bool fr_fixed = false;
            real_T fr_max = nom_ang_rad;
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

            //rear rotor has different parameters than the front two
            RotorTiltableParams r_params = RotorTiltableParams();
            r_params.rotor_params.calculateMaxThrust();
            r_params.prop_diameter = 5.5 * 0.0254;
            r_params.motor_resistance = 0.4;
            r_params.motor_KV = 1550;
            r_params.motor_KQ = (1.0 / r_params.motor_KV) * 60.0 / (2.0 * M_PIf);
            r_params.no_load_current = 0.6;
            r_params.CT0 = 0.2097;
            r_params.CT1 = 0.0505;
            r_params.CT2 = -0.1921;
            r_params.CQ0 = 0.0216;
            r_params.CQ1 = 0.0292;
            r_params.CQ2 = -0.0368;

            RotorTiltableConfiguration r_config(r_pos, r_norm, r_rot, r_fixed, r_max, r_direction, r_params);
            params.rotor_configs.push_back(r_config);

            params.mass = 0.7f;
            params.inertia << 0.0165, 0.f, 0.000048,
                0.f, 0.025, 0.f,
                0.000048, 0.f, 0.0282;

            params.aero_params = AeroParams();
            params.aero_params.aero_control_mixer = (Matrix3x3r() << -0.5f, 0.5f, 0.0f, //note that some signs are flipped because on actual aircraft,
                                                     0.5f,
                                                     0.5f,
                                                     0.0f, //+1 makes left elevon go down and +1 makes right elevon go up
                                                     0.0f,
                                                     0.0f,
                                                     0.0f)
                                                        .finished(); //convergence aircraft has no rudder
        }

    private:
        Params params_;
        SensorCollection sensors_; //maintains sensor type indexed collection of sensors
        vector<shared_ptr<SensorBase>> sensor_storage_; //RAII for created sensors
    };
}
} //namespace
#endif
