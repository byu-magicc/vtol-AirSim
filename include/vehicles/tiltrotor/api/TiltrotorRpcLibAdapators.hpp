// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_TiltrotorRpcLibAdaptors_hpp
#define air_TiltrotorRpcLibAdaptors_hpp

#include "common/Common.hpp"
#include "common/CommonStructs.hpp"
#include "api/RpcLibAdapatorsBase.hpp"
#include "vehicles/tiltrotor/api/TiltrotorCommon.hpp"
#include "vehicles/tiltrotor/api/TiltrotorApiBase.hpp"
#include "common/ImageCaptureBase.hpp"
#include "safety/SafetyEval.hpp"

#include "common/common_utils/WindowsApisCommonPre.hpp"
#include "rpc/msgpack.hpp"
#include "common/common_utils/WindowsApisCommonPost.hpp"

namespace msr { namespace airlib_rpclib {

class TiltrotorRpcLibAdaptors : public RpcLibAdapatorsBase {
public:
    struct VTOLYawMode {
        bool is_rate = true;
        float yaw_or_rate = 0;
        MSGPACK_DEFINE_MAP(is_rate, yaw_or_rate);

        VTOLYawMode()
        {}

        VTOLYawMode(const msr::airlib::VTOLYawMode& s)
        {
            is_rate = s.is_rate;
            yaw_or_rate = s.yaw_or_rate;
        }
        msr::airlib::VTOLYawMode to() const
        {
            return msr::airlib::VTOLYawMode(is_rate, yaw_or_rate);
        }
    };

    struct TiltrotorState {
        CollisionInfo collision;
        KinematicsState kinematics_estimated;
        KinematicsState kinematics_true;
        GeoPoint gps_location;
        uint64_t timestamp;
        VTOLLandedState landed_state;
        RCData rc_data;
        bool ready;
        std::string ready_message;
        std::vector<std::string> controller_messages;
        bool can_arm;

        MSGPACK_DEFINE_MAP(collision, kinematics_estimated, gps_location, timestamp, landed_state, rc_data);

        TiltrotorState()
        {}

        TiltrotorState(const msr::airlib::TiltrotorState& s)
        {
            collision = s.collision;
            kinematics_estimated = s.kinematics_estimated;
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
            return msr::airlib::TiltrotorState(collision.to(), kinematics_estimated.to(),
                gps_location.to(), timestamp, landed_state, rc_data.to(), ready, ready_message, can_arm);
        }
    };
};

}} //namespace

MSGPACK_ADD_ENUM(msr::airlib::VTOLDrivetrainType);
MSGPACK_ADD_ENUM(msr::airlib::VTOLLandedState);


#endif
