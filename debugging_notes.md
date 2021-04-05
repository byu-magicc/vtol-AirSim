# NaNs found in:
+ current_wrench_
+ tilt_output_.rotor_output
+ airspeed_body_vector_

*happens sometime before this backtrace*
  * msr::airlib::Rotor::update(this=0x00007ffeb3ddf6e0) at Rotor.hpp:93:9
    msr::airlib::RotorTiltable::update(this=0x00007ffeb3ddf6e0) at RotorTiltable.hpp:103:16
    msr::airlib::PhysicsBody::update(this=0x00007fff7f8bb680) at PhysicsBody.hpp:128:43
    msr::airlib::AeroBody::update(this=0x00007fff7f8bb680) at AeroBody.hpp:41:22
    TiltrotorPawnSimApi::update(this=0x00007ffeb28bad20) at TiltrotorPawnSimApi.cpp:158:25
    msr::airlib::UpdatableContainer<msr::airlib::UpdatableObject*>::update(this=0x00007ffeb3f35cd0) at UpdatableContainer.hpp:45:21
    msr::airlib::World::update(this=0x00007ffeb3f35cd0) at World.hpp:43:29
    msr::airlib::World::worldUpdatorAsync(this=0x00007ffeb3f35cd0, dt_nanos=3000925) at World.hpp:132:13
    ....