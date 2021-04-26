#pragma once

#include "interfaces/IController.hpp"
#include "interfaces/IStateEstimator.hpp"
#include "interfaces/ICommLink.hpp"
#include "interfaces/IGoal.hpp"

namespace tiltrotor_simple{

//This controller literally does nothing. It just implements the pure virtual IController functions.
//Use it if you need to use Passthrough commands but you have more than 4 actuator inputs, and 
//call the commandPWMs api function which overrides the actuator outputs in the firmware.
class DoNothingController : public IController {
public:
    virtual void initialize(const IGoal* goal, const IStateEstimator* state_estimator) override
    {
        unused(goal);
        unused(state_estimator);
    }
    virtual const Axis4r& getOutput() override
    {
        return Axis4r();
    }
    virtual bool isLastGoalModeAllPassthrough() override
    {
        return true;
    }
};

} //namespace