// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_RotorTiltableParams_hpp
#define msr_airlib_RotorTiltableParams_hpp


#include "common/Common.hpp"

namespace msr {
    namespace airlib {


        struct RotorTiltrotorParams {
            
            bool use_simple_rotor_model = false;  //if true, will use method of Rotor to calculate thrust and torque.
                                                  //otherwise, will use more complicated model below

            RotorParams rotor_params = RotorParams(); //only some params will be used if use_simple_rotor_model is false

            real_T angle_signal_filter_tc = 0.005f;    //time constant for angle command signal
            real_T angle_filter_tc = 0.1f;    //time constant for servo arm to reach angle command signal

            //rotor aerodynamic params
            real_T max_voltage = ;
            real_T prop_diameter = ;
            real_T motor_resistance = ;
            real_T air_density = ;
            real_T motor_KQ = ;
            real_T no_load_current = ;

            real_T CT0 = ;
            real_T CT1 = ;
            real_T CT2 = ;
            real_T CQ0 = ;
            real_T CQ1 = ;
            real_T CQ2 = ;

        };


    }
} //namespace
#endif
