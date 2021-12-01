// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

//in header only mode, control library is not available
#ifndef AIRLIB_HEADER_ONLY
//RPC code requires C++14. If build system like Unreal doesn't support it then use compiled binaries
#ifndef AIRLIB_NO_RPC
//if using Unreal Build system then include precompiled header file first

#include "vehicles/vtol/api/VtolRpcLibClient.hpp"

#include "common/Common.hpp"
#include <thread>
STRICT_MODE_OFF

#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK

#ifdef nil
#undef nil
#endif // nil

#include "common/common_utils/WindowsApisCommonPre.hpp"
#undef FLOAT
#undef check
#include "rpc/client.h"
//TODO: HACK: UE4 defines macro with stupid names like "check" that conflicts with msgpack library
#ifndef check
#define check(expr) (static_cast<void>((expr)))
#endif
#include "common/common_utils/WindowsApisCommonPost.hpp"

#include "vehicles/vtol/api/VtolRpcLibAdaptors.hpp"
#include "vehicles/multirotor/api/MultirotorRpcLibAdaptors.hpp"

STRICT_MODE_ON
#ifdef _MSC_VER
__pragma(warning(disable : 4239))
#endif

    namespace msr
{
    namespace airlib
    {

        typedef msr::airlib_rpclib::VtolRpcLibAdaptors VtolRpcLibAdaptors;
        typedef msr::airlib_rpclib::MultirotorRpcLibAdaptors MultirotorRpcLibAdaptors;

        struct VtolRpcLibClient::impl
        {
        public:
            std::future<RPCLIB_MSGPACK::object_handle> last_future;
        };

        VtolRpcLibClient::VtolRpcLibClient(const string& ip_address, uint16_t port, float timeout_sec)
            : RpcLibClientBase(ip_address, port, timeout_sec)
        {
            pimpl_.reset(new impl());
        }

        VtolRpcLibClient::~VtolRpcLibClient()
        {
        }

        VtolRpcLibClient* VtolRpcLibClient::takeoffAsync(float timeout_sec, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("takeoff", timeout_sec, vehicle_name);
            return this;
        }
        VtolRpcLibClient* VtolRpcLibClient::landAsync(float timeout_sec, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("land", timeout_sec, vehicle_name);
            return this;
        }
        VtolRpcLibClient* VtolRpcLibClient::goHomeAsync(float timeout_sec, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("goHome", timeout_sec, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByVelocityBodyFrameAsync(float vx, float vy, float vz, float duration,
                                                                                   DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByVelocityBodyFrame", vx, vy, vz, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByVelocityZBodyFrameAsync(float vx, float vy, float z, float duration,
                                                                                    DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByVelocityZBodyFrame", vx, vy, z, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByMotorPWMsAsync(const vector<float>& pwm_values, float duration, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByMotorPWMs", pwm_values, duration, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByRollPitchYawZAsync(float roll, float pitch, float yaw, float z, float duration, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByRollPitchYawZ", roll, pitch, yaw, z, duration, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByRollPitchYawThrottleAsync(float roll, float pitch, float yaw, float throttle, float duration, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByRollPitchYawThrottle", roll, pitch, yaw, throttle, duration, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByRollPitchYawrateThrottleAsync(float roll, float pitch, float yaw_rate, float throttle, float duration, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByRollPitchYawrateThrottle", roll, pitch, yaw_rate, throttle, duration, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByRollPitchYawrateZAsync(float roll, float pitch, float yaw_rate, float z, float duration, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByRollPitchYawrateZ", roll, pitch, yaw_rate, z, duration, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByAngleRatesZAsync(float roll_rate, float pitch_rate, float yaw_rate, float z, float duration, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByAngleRatesZ", roll_rate, pitch_rate, yaw_rate, z, duration, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByAngleRatesThrottleAsync(float roll_rate, float pitch_rate, float yaw_rate, float throttle, float duration, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByAngleRatesThrottle", roll_rate, pitch_rate, yaw_rate, throttle, duration, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByVelocityAsync(float vx, float vy, float vz, float duration,
                                                                          DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByVelocity", vx, vy, vz, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByVelocityZAsync(float vx, float vy, float z, float duration,
                                                                           DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByVelocityZ", vx, vy, z, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveOnPathAsync(const vector<Vector3r>& path, float velocity, float duration,
                                                                      DrivetrainType drivetrain, const YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name)
        {
            vector<VtolRpcLibAdaptors::Vector3r> conv_path;
            VtolRpcLibAdaptors::from(path, conv_path);
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveOnPath", conv_path, velocity, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), lookahead, adaptive_lookahead, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveToPositionAsync(float x, float y, float z, float velocity, float timeout_sec,
                                                                          DrivetrainType drivetrain, const YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveToPosition", x, y, z, velocity, timeout_sec, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), lookahead, adaptive_lookahead, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveToZAsync(float z, float velocity, float timeout_sec, const YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveToZ", z, velocity, timeout_sec, MultirotorRpcLibAdaptors::YawMode(yaw_mode), lookahead, adaptive_lookahead, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::moveByManualAsync(float vx_max, float vy_max, float z_min, float duration,
                                                                        DrivetrainType drivetrain, const YawMode& yaw_mode, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("moveByManual", vx_max, vy_max, z_min, duration, drivetrain, MultirotorRpcLibAdaptors::YawMode(yaw_mode), vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::rotateToYawAsync(float yaw, float timeout_sec, float margin, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("rotateToYaw", yaw, timeout_sec, margin, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::rotateByYawRateAsync(float yaw_rate, float duration, const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("rotateByYawRate", yaw_rate, duration, vehicle_name);
            return this;
        }

        VtolRpcLibClient* VtolRpcLibClient::hoverAsync(const std::string& vehicle_name)
        {
            pimpl_->last_future = static_cast<rpc::client*>(getClient())->async_call("hover", vehicle_name);
            return this;
        }

        void VtolRpcLibClient::setAngleLevelControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name)
        {
            static_cast<rpc::client*>(getClient())->call("setAngleLevelControllerGains", kp, ki, kd, vehicle_name);
        }

        void VtolRpcLibClient::setAngleRateControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name)
        {
            static_cast<rpc::client*>(getClient())->call("setAngleRateControllerGains", kp, ki, kd, vehicle_name);
        }

        void VtolRpcLibClient::setVelocityControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name)
        {
            static_cast<rpc::client*>(getClient())->call("setVelocityControllerGains", kp, ki, kd, vehicle_name);
        }

        void VtolRpcLibClient::setPositionControllerGains(const vector<float>& kp, const vector<float>& ki, const vector<float>& kd, const std::string& vehicle_name)
        {
            static_cast<rpc::client*>(getClient())->call("setPositionControllerGains", kp, ki, kd, vehicle_name);
        }

        bool VtolRpcLibClient::setSafety(SafetyEval::SafetyViolationType enable_reasons, float obs_clearance, SafetyEval::ObsAvoidanceStrategy obs_startegy,
                                              float obs_avoidance_vel, const Vector3r& origin, float xy_length, float max_z, float min_z, const std::string& vehicle_name)
        {
            return static_cast<rpc::client*>(getClient())->call("setSafety", static_cast<uint>(enable_reasons), obs_clearance, obs_startegy, obs_avoidance_vel, VtolRpcLibAdaptors::Vector3r(origin), xy_length, max_z, min_z, vehicle_name).as<bool>();
        }

        //status getters
        // Rotor state getter
        RotorTiltableStates VtolRpcLibClient::getRotorStates(const std::string& vehicle_name)
        {
            return static_cast<rpc::client*>(getClient())->call("getRotorStates", vehicle_name).as<VtolRpcLibAdaptors::RotorTiltableStates>().to();
        }
        // Vtol state getter
        VtolState VtolRpcLibClient::getVtolState(const std::string& vehicle_name)
        {
            return static_cast<rpc::client*>(getClient())->call("getVtolState", vehicle_name).as<VtolRpcLibAdaptors::VtolState>().to();
        }

        void VtolRpcLibClient::moveByRC(const RCData& rc_data, const std::string& vehicle_name)
        {
            static_cast<rpc::client*>(getClient())->call("moveByRC", VtolRpcLibAdaptors::RCData(rc_data), vehicle_name);
        }

        //return value of last task. It should be true if task completed without
        //cancellation or timeout
        VtolRpcLibClient* VtolRpcLibClient::waitOnLastTask(bool* task_result, float timeout_sec)
        {
            bool result;
            if (std::isnan(timeout_sec) || timeout_sec == Utils::max<float>())
                result = pimpl_->last_future.get().as<bool>();
            else {
                auto future_status = pimpl_->last_future.wait_for(std::chrono::duration<double>(timeout_sec));
                if (future_status == std::future_status::ready)
                    result = pimpl_->last_future.get().as<bool>();
                else
                    result = false;
            }

            if (task_result)
                *task_result = result;

            return this;
        }
    }
} //namespace

#endif
#endif
