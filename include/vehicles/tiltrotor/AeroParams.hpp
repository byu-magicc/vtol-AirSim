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

        //control mixers
        //ElevatorAileronRudderMixer: mix from {elevator, aileron, rudder} to {elevator, aileron, rudder}
        //AileronRudderVatorMixer: mix from {aileron, right ruddervator, left ruddervator} to {elevator, aileron, rudder}
        //ElevonRudderMixer: mix from {right elevon, left elevon, rudder} to {elevator, aileron, rudder}
        Matrix3x3r ElevatorAileronRudderMixer = Matrix3x3r::Identity();

        Matrix3x3r AileronRudderVatorMixer;
        AileronRudderVatorMixer << 0.f, 1.f, 1.f,
                            1.f, 0.f, 0.f,
                            0.f, -1.f, 1.f;

        Matrix3x3r ElevonRudderMixer;
        ElevonRudderMixer << 1.f, 1.f, 0.f,
                       -1.f, 1.f, 0.f,
                        0.f, 0.f, 1.f;

        static vector<Matrix3x3r> AeroControlMixers;
        AeroControlMixers.push_back(ElevatorAileronRudderMixer);
        AeroControlMixers.push_back(AileronRudderVatorMixer);
        AeroControlMixers.push_back(ElevonRudderMixer);

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
                  alpha = std::atan2(airspeed(2) , airspeed(0));
                if (Va == 0.f)
                  beta = VectorMath::(airspeed(1)) * M_PI / 2.0;
                else
                  beta = std::asin(airspeed(1)/Va);
            }

            void setAirDensity(real_T air_density)
            {
                rho = air_density;
            }
        };

        struct AeroCoefficient {
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

        struct AeroParams {
            //flap parameters
            real_T flap_rise_time = 0.01;
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

            //aerodynamic coefficients
            AeroCoefficient CL;
            CL.O = 0.005;
            CL.alpha = 2.819;
            CL.beta = 0.0;
            CL.p = 0.0;
            CL.q = 3.242;
            CL.r = 0.0;
            CL.delta_a = 0.0;
            CL.delta_e = 0.2;
            CL.delta_r = 0.0;

            AeroCoefficient CD;
            CD.O = 0.022;
            CD.alpha = 0.3;
            CD.beta = 0.0;
            CD.p = 0.05; //used as parasitic drag
            CD.q = 0.0;
            CD.r = 0.0;
            CD.delta_a = 0.0;
            CD.delta_e = 0.5;
            CD.delta_r = 0.0;

            AeroCoefficient CY;
            CY.O = 0.0;
            CY.alpha = 0.0;
            CY.beta = -0.318;
            CY.p = 0.078;
            CY.q = 0.0;
            CY.r = 0.288;
            CY.delta_a = 0.000536;
            CY.delta_e = 0.0;
            CY.delta_r = 0.0;

            AeroCoefficient Cl;
            Cl.O = 0.0;
            Cl.alpha = 0.0;
            Cl.beta = -0.032;
            Cl.p = -0.207;
            Cl.q = 0.0;
            Cl.r = 0.036;
            Cl.delta_a = 0.018;
            Cl.delta_e = 0.0;
            Cl.delta_r = 0.0;

            AeroCoefficient Cm;
            Cm.O = 0.0;
            Cm.alpha = -0.185;
            Cm.beta = 0.0;
            Cm.p = 0.0;
            Cm.q = -1.093;
            Cm.r = 0.0;
            Cm.delta_a = 0.0;
            Cm.delta_e = -0.05;
            Cm.delta_r = 0.0;

            AeroCoefficient Cn;
            Cn.O = 0.0;
            Cn.alpha = 0.0;
            Cn.beta = 0.112;
            Cn.p = -0.053;
            Cn.q = 0.0;
            Cn.r = -0.104;
            Cn.delta_a = -0.00328;
            Cn.delta_e = 0.0;
            Cn.delta_r = 0.0;
        };

    }
} //namespace

#endif