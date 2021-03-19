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
            real_T S = ;
            real_T b = ;
            real_T c = ;
            real_T aspect_ratio = pow(b, 2.f) / S;
            real_T epsilon = ;
            real_T alpha0 = ;
            real_T M = ;
            real_T e = 0.9;

            //aerodynamic coefficients
            AeroCoefficient CL;
            CL.O = ;
            CL.alpha = ;
            CL.beta = ;
            CL.p = ;
            CL.q = ;
            CL.r = ;
            CL.delta_a = ;
            CL.delta_e = ;
            CL.delta_r = ;

            AeroCoefficient CD;
            CD.O = ;
            CD.alpha = ;
            CD.beta = ;
            CD.p = ;
            CD.q = ;
            CD.r = ;
            CD.delta_a = ;
            CD.delta_e = ;
            CD.delta_r = ;

            AeroCoefficient Cl;
            Cl.O = ;
            Cl.alpha = ;
            Cl.beta = ;
            Cl.p = ;
            Cl.q = ;
            Cl.r = ;
            Cl.delta_a = ;
            Cl.delta_e = ;
            Cl.delta_r = ;

            AeroCoefficient Cm;
            Cm.O = ;
            Cm.alpha = ;
            Cm.beta = ;
            Cm.p = ;
            Cm.q = ;
            Cm.r = ;
            Cm.delta_a = ;
            Cm.delta_e = ;
            Cm.delta_r = ;

            AeroCoefficient Cn;
            Cn.O = ;
            Cn.alpha = ;
            Cn.beta = ;
            Cn.p = ;
            Cn.q = ;
            Cn.r = ;
            Cn.delta_a = ;
            Cn.delta_e = ;
            Cn.delta_r = ;
        };

    }
} //namespace

#endif