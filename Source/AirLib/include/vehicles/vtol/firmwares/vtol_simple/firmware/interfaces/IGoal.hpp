#pragma once

#include "IUpdatable.hpp"
#include "CommonStructs.hpp"

namespace vtol_simple
{

class IGoal
{
public:
    virtual const Axis4r& getGoalValue() const = 0;
    virtual const GoalMode& getGoalMode() const = 0;
};

} //namespace