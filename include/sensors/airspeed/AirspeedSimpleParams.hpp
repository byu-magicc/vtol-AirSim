// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_AirspeedSimpleParams_hpp
#define msr_airlib_AirspeedSimpleParams_hpp

#include "common/Common.hpp"


namespace msr { namespace airlib {


struct AirspeedSimpleParams {
    // not using random walk
    // real_T pressure_factor_sigma = 0.0365f / 20;
    // real_T pressure_factor_tau = 3600;

    //value from mavsim
    real_T unnorrelated_noise_sigma = 0.002f * 100;

    //see PX4 param reference for EKF: https://dev.px4.io/en/advanced/parameter_reference.html
    real_T update_latency = 0.0f;    //sec
    real_T update_frequency = 50;    //Hz
    real_T startup_delay = 0;        //sec

    void initializeFromSettings(const AirSimSettings::AirspeedSetting& settings)
    {
        unused(settings);
    }
};


}} //namespace
#endif
