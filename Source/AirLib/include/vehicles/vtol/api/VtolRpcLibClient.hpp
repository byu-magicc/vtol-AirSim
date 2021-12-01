// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_VtolRpcLibClient_hpp
#define air_VtolRpcLibClient_hpp

#include "common/Common.hpp"
#include <functional>
#include "common/CommonStructs.hpp"
#include "common/ImageCaptureBase.hpp"
#include "vehicles/vtol/api/VtolApiBase.hpp"
#include "api/RpcLibClientBase.hpp"
#include "vehicles/vtol/api/VtolCommon.hpp"
#include "vehicles/vtol/api/VtolCommon.hpp"

namespace msr
{
namespace airlib
{

    class VtolRpcLibClient : public RpcLibClientBase
    {
    public:
        VtolRpcLibClient(const string& ip_address = "localhost", uint16_t port = RpcLibPort, float timeout_sec = 60);

        VtolRpcLibClient* takeoffAsync(float timeout_sec = 20, const std::string& vehicle_name = "");
        VtolRpcLibClient* landAsync(float timeout_sec = 60, const std::string& vehicle_name = "");
        VtolRpcLibClient* goHomeAsync(float timeout_sec = Utils::max<float>(), const std::string& vehicle_name = "");

        VtolRpcLibClient* moveByVelocityBodyFrameAsync(float vx, float vy, float vz, float duration,
                                                            DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByVelocityZBodyFrameAsync(float vx, float vy, float z, float duration,
                                                             DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByRollPitchYawZAsync(float roll, float pitch, float yaw, float z, float duration, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByRollPitchYawThrottleAsync(float roll, float pitch, float yaw, float throttle, float duration, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByRollPitchYawrateThrottleAsync(float roll, float pitch, float yaw_rate, float throttle, float duration, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByRollPitchYawrateZAsync(float roll, float pitch, float yaw_rate, float z, float duration, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByAngleRatesZAsync(float roll_rate, float pitch_rate, float yaw_rate, float z, float duration, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByAngleRatesThrottleAsync(float roll_rate, float pitch_rate, float yaw_rate, float throttle, float duration, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByVelocityAsync(float vx, float vy, float vz, float duration,
                                                   DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByVelocityZAsync(float vx, float vy, float z, float duration,
                                                    DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        VtolRpcLibClient* moveOnPathAsync(const vector<Vector3r>& path, float velocity, float timeout_sec = Utils::max<float>(),
                                               DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(),
                                               float lookahead = -1, float adaptive_lookahead = 1, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveToPositionAsync(float x, float y, float z, float velocity, float timeout_sec = Utils::max<float>(),
                                                   DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(),
                                                   float lookahead = -1, float adaptive_lookahead = 1, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveToZAsync(float z, float velocity, float timeout_sec = Utils::max<float>(),
                                            const YawMode& yaw_mode = YawMode(), float lookahead = -1, float adaptive_lookahead = 1, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByManualAsync(float vx_max, float vy_max, float z_min, float duration,
                                                 DrivetrainType drivetrain = DrivetrainType::MaxDegreeOfFreedom, const YawMode& yaw_mode = YawMode(), const std::string& vehicle_name = "");
        VtolRpcLibClient* rotateToYawAsync(float yaw, float timeout_sec = Utils::max<float>(), float margin = 5, const std::string& vehicle_name = "");
        VtolRpcLibClient* rotateByYawRateAsync(float yaw_rate, float duration, const std::string& vehicle_name = "");
        VtolRpcLibClient* hoverAsync(const std::string& vehicle_name = "");

        VtolRpcLibClient* setVtolPose(Pose pose, const vector<float>& tilt_angles, bool ignore_collision, const std::string& vehicle_name = "");
        VtolRpcLibClient* moveByMotorPWMsAsync(const vector<float>& pwm_values, float duration, const std::string& vehicle_name = "");

        void setAngleLevelControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name = "");
        void setAngleRateControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name = "");
        void setVelocityControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name = "");
        void setPositionControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name = "");
        void moveByRC(const RCData& rc_data, const std::string& vehicle_name = "");

        VtolState getVtolState(const std::string& vehicle_name = "");
        RotorTiltableStates getRotorStates(const std::string& vehicle_name = "");

        bool setSafety(SafetyEval::SafetyViolationType enable_reasons, float obs_clearance, SafetyEval::ObsAvoidanceStrategy obs_startegy,
                       float obs_avoidance_vel, const Vector3r& origin, float xy_length, float max_z, float min_z, const std::string& vehicle_name = "");

        virtual VtolRpcLibClient* waitOnLastTask(bool* task_result = nullptr, float timeout_sec = Utils::nan<float>()) override;

        virtual ~VtolRpcLibClient(); //required for pimpl

    private:
        struct impl;
        std::unique_ptr<impl> pimpl_;
    };
}
} //namespace
#endif
