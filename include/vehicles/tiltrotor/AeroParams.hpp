// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_AeroParams_hpp
#define msr_airlib_AeroParams_hpp


#include "common/Common.hpp"

namespace msr {
    namespace airlib {

        enum class AeroControlType : int {
            ElevatorAileronRudder = 0,
            AileronRudderVator = 1,
            ElevonRudder = 2
        };

        //contains air params necessary for calculating aerodynamic forces and moments
        struct AirState {
            Vector3r airspeed_vector;
            real_T Va;
            real_T alpha;
            real_T beta;
            real_T rho;

            void zero()
            {
                airspeed_vector.setZero();
                Va = 0.f;
                alpha = 0.f;
                beta = 0.f;
                rho = 0.f;
            }

            void setAirspeed(Vector3r airspeed)
            {
                airspeed_vector = airspeed;
                Va = airspeed.norm();

                if (airspeed(0) == 0.f)
                  alpha = VectorMath::sgn(airspeed(2)) * M_PI / 2.0;
                else
                  alpha = std::atan2(airspeed(2), airspeed(0));
                if (Va == 0.f)
                  beta = VectorMath::sgn(airspeed(1)) * M_PI / 2.0;
                else
                  beta = std::asin(airspeed(1)/Va);
            }

            void setAirDensity(real_T air_density)
            {
                rho = air_density;
            }
        };

        struct AeroParams {
            //flap parameters
            real_T flap_rise_time = 0.05f;
            real_T flap_max_angle = M_PIf/6.0;

            //wing parameters
            real_T S = 0.2589;
            real_T b = 1.4224;
            real_T c = 0.3305;
            real_T aspect_ratio = pow(b, 2.f) / S;
            real_T epsilon = 0.16;
            real_T alpha0 = 0.47;
            real_T M = 50.0;
            real_T e = 0.9;

            struct AeroCoefficient {
                AeroCoefficient(real_T O_in, real_T alpha_in, real_T beta_in, real_T p_in, real_T q_in, real_T r_in, 
                    real_T delta_a_in, real_T delta_e_in, real_T delta_r_in) :
                    O{O_in}, alpha{alpha_in}, beta{beta_in}, p{p_in}, q{q_in}, r{r_in}, delta_a{delta_a_in}, delta_e{delta_e_in}, delta_r{delta_r_in}
                {}

                real_T O;
                real_T alpha;
                real_T beta;
                real_T p;
                real_T q;
                real_T r;
                real_T delta_a;
                real_T delta_e;
                real_T delta_r;
            };

            // //aerodynamic coefficients
            AeroCoefficient CL{0.005, 2.819, 0.0, 0.0, 3.242, 0.0, 0.0, 0.2, 0.0};
            AeroCoefficient CD{0.022, 0.3, 0.0, 0.05, 0.0, 0.0, 0.0, 0.5, 0.0};
            AeroCoefficient CY{0.0, 0.0, -0.318, 0.078, 0.0, 0.288, 0.000536, 0.0, 0.0};
            AeroCoefficient Cl{0.0, 0.0, -0.032, -0.207, 0.0, 0.036, 0.018, 0.0, 0.0};
            AeroCoefficient Cm{0.0, -0.185, 0.0, 0.0, -1.093, 0.0, 0.0, -0.05, 0.0};
            AeroCoefficient Cn{0.0, 0.0, 0.112, -0.053, 0.0, -0.104, -0.00328, 0.0, 0.0};

            //control mixers
            //elevator_airleron_rudder_mixer: mix from {elevator, aileron, rudder} to {elevator, aileron, rudder}
            //aileron_ruddervator_mixer: mix from {aileron, right ruddervator, left ruddervator} to {elevator, aileron, rudder}
            //elevon_rudder_mixer: mix from {right elevon, left elevon, rudder} to {elevator, aileron, rudder}
            Matrix3x3r elevator_aileron_rudder_mixer = Matrix3x3r::Identity();

            Matrix3x3r aileron_ruddervator_mixer = (Matrix3x3r() << 0.f, 0.5f, 0.5f,
                                                                    1.f, 0.f, 0.f,
                                                                    0.f, -0.5f, 0.5f).finished();

            Matrix3x3r elevon_rudder_mixer = (Matrix3x3r() << 0.5f, 0.5f, 0.f,
                                                              -0.5f, 0.5f, 0.f,
                                                              0.f, 0.f, 1.f).finished();

            vector<Matrix3x3r> aero_control_mixers = {elevator_aileron_rudder_mixer, aileron_ruddervator_mixer, elevon_rudder_mixer};
        };

    }
} //namespace

#endif