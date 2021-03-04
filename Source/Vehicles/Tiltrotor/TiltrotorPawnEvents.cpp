#include "TiltrotorPawnEvents.h"

TiltrotorPawnEvents::ActuatorsSignal& TiltrotorPawnEvents::getActuatorSignal()
{
    return actuator_signal_;
}