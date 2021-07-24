// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_AirspeedSensor_hpp
#define msr_airlib_AirspeedSensor_hpp

#include <random>
#include "common/Common.hpp"
#include "AirspeedSimpleParams.hpp"
#include "AirspeedBase.hpp"
#include "common/GaussianMarkov.hpp"
#include "common/DelayLine.hpp"
#include "common/FrequencyLimiter.hpp"

namespace msr
{
namespace airlib
{

    class AirspeedSimple : public AirspeedBase
    {
    public:
        AirspeedSimple(const AirSimSettings::AirspeedSetting& setting = AirSimSettings::AirspeedSetting())
            : AirspeedBase(setting.sensor_name)
        {
            // initialize params
            params_.initializeFromSettings(setting);

            // GM process that would do random walk for pressure factor
            // pressure_factor_.initialize(params_.pressure_factor_tau, params_.pressure_factor_sigma, 0);

            uncorrelated_noise_ = RandomGeneratorGausianR(0.0f, params_.uncorrelated_noise_sigma);

            //initialize frequency limiter
            freq_limiter_.initialize(params_.update_frequency, params_.startup_delay);
            delay_line_.initialize(params_.update_latency);
        }

        //*** Start: UpdatableState implementation ***//
        virtual void resetImplementation() override
        {
            // pressure_factor_.reset();
            //correlated_noise_.reset();
            uncorrelated_noise_.reset();

            freq_limiter_.reset();
            delay_line_.reset();

            delay_line_.push_back(getOutputInternal());
        }

        virtual void update() override
        {
            AirspeedBase::update();

            freq_limiter_.update();

            if (freq_limiter_.isWaitComplete()) {
                delay_line_.push_back(getOutputInternal());
            }

            delay_line_.update();

            if (freq_limiter_.isWaitComplete())
                setOutput(delay_line_.getOutput());
        }
        //*** End: UpdatableState implementation ***//

        virtual ~AirspeedSimple() = default;

    private: //methods
        Output getOutputInternal()
        {
            Output output;
            const GroundTruth& ground_truth = getGroundTruth();

            auto air_density = ground_truth.environment->getState().air_density;
            auto airspeed = ground_truth.environment->getState().airspeed;

            real_T diff_pressure = air_density * (airspeed * airspeed) / 2.0f;

            diff_pressure += uncorrelated_noise_.next();

            output.diff_pressure = diff_pressure;

            output.time_stamp = clock()->nowNanos();

            return output;
        }

    private:
        AirspeedSimpleParams params_;

        // GaussianMarkov pressure_factor_;
        //GaussianMarkov correlated_noise_;
        RandomGeneratorGausianR uncorrelated_noise_;

        FrequencyLimiter freq_limiter_;
        DelayLine<Output> delay_line_;
    };

}
} //namespace
#endif
