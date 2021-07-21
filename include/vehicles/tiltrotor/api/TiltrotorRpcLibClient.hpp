// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_TiltrotorRpcLibClient_hpp
#define air_TiltrotorRpcLibClient_hpp

#include "common/Common.hpp"
#include <functional>
#include "common/CommonStructs.hpp"
#include "common/ImageCaptureBase.hpp"
#include "vehicles/tiltrotor/api/TiltrotorApiBase.hpp"
#include "api/RpcLibClientBase.hpp"
#include "vehicles/tiltrotor/api/TiltrotorCommon.hpp"
#include "vehicles/tiltrotor/api/TiltrotorCommon.hpp"

namespace msr
{
namespace airlib
{

    class TiltrotorRpcLibClient : public RpcLibClientBase
    {
    public:
        TiltrotorRpcLibClient(const string& ip_address = "localhost", uint16_t port = RpcLibPort, float timeout_sec = 60);

        TiltrotorRpcLibClient* takeoffAsync(float timeout_sec = 20, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* landAsync(float timeout_sec = 60, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* goHomeAsync(float timeout_sec = Utils::max<float>(), const std::string& vehicle_name = "");

        TiltrotorRpcLibClient* moveByVelocityBodyFrameAsync(float vx, float vy, float vz, float duration,
                                                            DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByVelocityZBodyFrameAsync(float vx, float vy, float z, float duration,
                                                             DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByRollPitchYawZAsync(float roll, float pitch, float yaw, float z, float duration, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByRollPitchYawThrottleAsync(float roll, float pitch, float yaw, float throttle, float duration, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByRollPitchYawrateThrottleAsync(float roll, float pitch, float yaw_rate, float throttle, float duration, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByRollPitchYawrateZAsync(float roll, float pitch, float yaw_rate, float z, float duration, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByAngleRatesZAsync(float roll_rate, float pitch_rate, float yaw_rate, float z, float duration, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByAngleRatesThrottleAsync(float roll_rate, float pitch_rate, float yaw_rate, float throttle, float duration, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByVelocityAsync(float vx, float vy, float vz, float duration,
                                                   DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByVelocityZAsync(float vx, float vy, float z, float duration,
                                                    DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveOnPathAsync(const vector<Vector3r>& path, float velocity, float timeout_sec = Utils::max<float>(),
                                               DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(),
                                               float lookahead = -1, float adaptive_lookahead = 1, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveToPositionAsync(float x, float y, float z, float velocity, float timeout_sec = Utils::max<float>(),
                                                   DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(),
                                                   float lookahead = -1, float adaptive_lookahead = 1, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveToZAsync(float z, float velocity, float timeout_sec = Utils::max<float>(),
                                            const YawMode& yaw_mode = YawMode(), float lookahead = -1, float adaptive_lookahead = 1, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByManualAsync(float vx_max, float vy_max, float z_min, float duration,
                                                 DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* rotateToYawAsync(float yaw, float timeout_sec = Utils::max<float>(), float margin = 5, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* rotateByYawRateAsync(float yaw_rate, float duration, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* hoverAsync(const std::string& vehicle_name = "");

        TiltrotorRpcLibClient* setTiltrotorPose(Pose pose, const vector<float>& tilt_angles, bool ignore_collision, const std::string& vehicle_name = "");
        TiltrotorRpcLibClient* moveByPWMsAsync(const vector<float>& pwm_values, float duration, const std::string& vehicle_name = "");

        void setAngleLevelControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name = "");
        void setAngleRateControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name = "");
        void setVelocityControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name = "");
        void setPositionControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name = "");
        void moveByRC(const RCData& rc_data, const std::string& vehicle_name = "");

        TiltrotorState getTiltrotorState(const std::string& vehicle_name = "");
        RotorTiltableStates getRotorStates(const std::string& vehicle_name = "");

        bool setSafety(SafetyEval::SafetyViolationType enable_reasons, float obs_clearance, SafetyEval::ObsAvoidanceStrategy obs_startegy,
                       float obs_avoidance_vel, const Vector3r& origin, float xy_length, float max_z, float min_z, const std::string& vehicle_name = "");

        virtual TiltrotorRpcLibClient* waitOnLastTask(bool* task_result = nullptr, float timeout_sec = Utils::nan<float>()) override;

        virtual ~TiltrotorRpcLibClient(); //required for pimpl

    private:
        struct impl;
        std::unique_ptr<impl> pimpl_;
    };

}
} //namespace
#endif
