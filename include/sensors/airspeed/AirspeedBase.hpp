// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_AirspeedBase_hpp
#define msr_airlib_AirspeedBase_hpp

#include "sensors/SensorBase.hpp"

namespace msr
{
namespace airlib
{

    class AirspeedBase : public SensorBase
    {
    public:
        AirspeedBase(const std::string& sensor_name = "")
            : SensorBase(sensor_name)
        {
        }

    public: //types
        struct Output
        {
            TTimePoint time_stamp;
            real_T diff_pressure; //Pascal
        };

    public:
        virtual void reportState(StateReporter& reporter) override
        {
            //call base
            UpdatableObject::reportState(reporter);

            reporter.writeValue("Airspeed-DiffP", output_.diff_pressure);
        }

        const Output& getOutput() const
        {
            return output_;
        }

    protected:
        void setOutput(const Output& output)
        {
            output_ = output;
        }

    private:
        Output output_;
    };

}
} //namespace
#endif
