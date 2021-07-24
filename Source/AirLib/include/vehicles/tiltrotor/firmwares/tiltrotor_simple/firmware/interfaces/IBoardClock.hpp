#pragma once

#include <cstdint>

namespace tiltrotor_simple
{

class IBoardClock
{
public:
    virtual uint64_t micros() const = 0;
    virtual uint64_t millis() const = 0;
};

}