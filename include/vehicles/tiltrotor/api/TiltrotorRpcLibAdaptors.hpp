// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_TiltrotorRpcLibAdaptors_hpp
#define air_TiltrotorRpcLibAdaptors_hpp

#include "common/Common.hpp"
#include "common/CommonStructs.hpp"
#include "api/RpcLibAdaptorsBase.hpp"
#include "vehicles/tiltrotor/api/TiltrotorCommon.hpp"
#include "vehicles/tiltrotor/api/TiltrotorApiBase.hpp"
#include "vehicles/multirotor/api/MultirotorCommon.hpp"
#include "common/ImageCaptureBase.hpp"
#include "safety/SafetyEval.hpp"

#include "common/common_utils/WindowsApisCommonPre.hpp"
#include "rpc/msgpack.hpp"
#include "common/common_utils/WindowsApisCommonPost.hpp"

namespace msr { namespace airlib_rpclib {

class TiltrotorRpcLibAdaptors : public RpcLibAdaptorsBase {
public:
    struct RotorTiltableParameters {
        msr::airlib::real_T thrust;
        msr::airlib::real_T torque_scaler;
        msr::airlib::real_T speed;
        msr::airlib::real_T angle;

        MSGPACK_DEFINE_MAP(thrust, torque_scaler, speed, angle);

        RotorTiltableParameters()
        {}

        RotorTiltableParameters(const msr::airlib::RotorTiltableParameters& s)
        {
            thrust = s.thrust;
            torque_scaler = s.torque_scaler;
            speed = s.speed;
            angle = s.angle;
        }

        msr::airlib::RotorTiltableParameters to() const
        {
            return msr::airlib::RotorTiltableParameters(thrust, torque_scaler, speed, angle);
        }
    };

    struct RotorTiltableStates {
        std::vector<RotorTiltableParameters> rotors;
        uint64_t timestamp;

        MSGPACK_DEFINE_MAP(rotors, timestamp);

        RotorTiltableStates()
        {}

        RotorTiltableStates(const msr::airlib::RotorTiltableStates& s)
        {
            for (const auto& r : s.rotors)
            {
                rotors.push_back(RotorTiltableParameters(r));
            }
            timestamp = s.timestamp;
        }

        msr::airlib::RotorTiltableStates to() const
        {
            std::vector<msr::airlib::RotorTiltableParameters> d;
            for (const auto& r : rotors)
            {
                d.push_back(r.to());
            }
            return msr::airlib::RotorTiltableStates(d, timestamp);
        }
    };

    struct TiltrotorState {
        CollisionInfo collision;
        KinematicsState kinematics_estimated;
        KinematicsState kinematics_true;
        GeoPoint gps_location;
        uint64_t timestamp;
        LandedState landed_state;
        RCData rc_data;
        bool ready;
        std::string ready_message;
        bool can_arm;

        MSGPACK_DEFINE_MAP(collision, kinematics_estimated, kinematics_true, gps_location,
            timestamp, landed_state, rc_data, ready, ready_message, can_arm);

        TiltrotorState()
        {}

        TiltrotorState(const msr::airlib::TiltrotorState& s)
        {
            collision = s.collision;
            kinematics_estimated = s.kinematics_estimated;
            kinematics_true = s.kinematics_true;
            gps_location = s.gps_location;
            timestamp = s.timestamp;
            landed_state = s.landed_state;
            rc_data = RCData(s.rc_data);
            ready = s.ready;
            ready_message = s.ready_message;
            can_arm = s.can_arm;
        }

        msr::airlib::TiltrotorState to() const
        {
            return msr::airlib::TiltrotorState(collision.to(), kinematics_estimated.to(), kinematics_true.to(),
                gps_location.to(), timestamp, landed_state, rc_data.to(), ready, ready_message, can_arm);
        }
    };
};

}} //namespace

#endif
