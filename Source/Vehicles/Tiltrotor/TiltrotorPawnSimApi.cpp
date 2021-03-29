#include "TiltrotorPawnSimApi.h"
#include "AirBlueprintLib.h"
#include "vehicles/tiltrotor/AeroBodyParamsFactory.hpp"
#include "UnrealSensors/UnrealSensorFactory.h"
#include <exception>

using namespace msr::airlib;

TiltrotorPawnSimApi::TiltrotorPawnSimApi(const Params& params)
    : PawnSimApi(params),
      pawn_events_(static_cast<TiltrotorPawnEvents*>(params.pawn_events))
{
    //reset roll & pitch of vehicle as multirotors required to be on plain surface at start
    Pose pose = getPose();
    float pitch, roll, yaw;
    VectorMath::toEulerianAngle(pose.orientation, pitch, roll, yaw);
    pose.orientation = VectorMath::toQuaternion(0, 0, yaw);
    setPose(pose, false);
}

void TiltrotorPawnSimApi::initialize()
{
    PawnSimApi::initialize();

    //create vehicle API
    std::shared_ptr<UnrealSensorFactory> sensor_factory = std::make_shared<UnrealSensorFactory>(getPawn(), &getNedTransform());
    vehicle_params_ = AeroBodyParamsFactory::createConfig(getVehicleSetting(), sensor_factory);
    vehicle_api_ = vehicle_params_->createTiltrotorApi();
    //setup physics vehicle
    aero_physics_body_ = std::unique_ptr<AeroBody>(new AeroBody(vehicle_params_.get(), vehicle_api_.get(),
        getKinematics(), getEnvironment()));
    rotor_count_ = aero_physics_body_->rotorCount();
    rotor_info_.assign(rotor_count_, RotorTiltableInfo());

    vehicle_api_->setSimulatedGroundTruth(getGroundTruthKinematics(), getGroundTruthEnvironment());

    //initialize private vars
    last_phys_pose_ = pending_phys_pose_ = Pose::nanPose();
    pending_pose_status_ = PendingPoseStatus::NonePending;
    reset_pending_ = false;
    did_reset_ = false;
}

void TiltrotorPawnSimApi::pawnTick(float dt)
{
    unused(dt);
    //calls to update* are handled by physics engine and in SimModeWorldBase
}

void TiltrotorPawnSimApi::updateRenderedState(float dt)
{
    //Utils::log("------Render tick-------");

    //if reset is pending then do it first, no need to do other things until next tick
    if (reset_pending_) {
        reset_task_();
        did_reset_ = true;
        return;
    }

    //move collision info from rendering engine to vehicle
    const CollisionInfo& collision_info = getCollisionInfo();
    aero_physics_body_->setCollisionInfo(collision_info);

    if (pending_pose_status_ == PendingPoseStatus::RenderStatePending) {
        aero_physics_body_->setPose(pending_phys_pose_);
        pending_pose_status_ = PendingPoseStatus::RenderPending;
    }

    last_phys_pose_ = aero_physics_body_->getPose();

    collision_response = aero_physics_body_->getCollisionResponseInfo();

    //update rotor poses
    for (int i = 0; i < rotor_count_; ++i) {
        const auto& output = aero_physics_body_->getRotorOutput(i);
        RotorTiltableInfo* info = &rotor_info_[i];
        info->rotor_speed = output.rotor_output.speed;
        info->rotor_direction = static_cast<int>(output.rotor_output.turning_direction);
        info->rotor_thrust = output.rotor_output.thrust;
        info->rotor_control_filtered = output.rotor_output.control_signal_filtered;
        info->rotor_angle_from_vertical = output.angle_from_vertical;
        info->is_fixed = output.is_fixed;
    }

    vehicle_api_->getStatusMessages(vehicle_api_messages_);

    if (getRemoteControlID() >= 0)
        vehicle_api_->setRCData(getRCData());
}

void TiltrotorPawnSimApi::updateRendering(float dt)
{
    //if we did reset then don't worry about synchronizing states for this tick
    if (reset_pending_) {
        // Continue to wait for reset
        if (!did_reset_) {
            return;
        }
        else {
            reset_pending_ = false;
            did_reset_ = false;
            return;
        }
    }

    if (!VectorMath::hasNan(last_phys_pose_)) {
        if (pending_pose_status_ ==  PendingPoseStatus::RenderPending) {
            PawnSimApi::setPose(last_phys_pose_, pending_pose_collisions_);
            pending_pose_status_ = PendingPoseStatus::NonePending;
        }
        else
            PawnSimApi::setPose(last_phys_pose_, false);
    }

    //UAirBlueprintLib::LogMessage(TEXT("Collision (raw) Count:"), FString::FromInt(collision_response.collision_count_raw), LogDebugLevel::Unimportant);
    UAirBlueprintLib::LogMessage(TEXT("Collision Count:"),
        FString::FromInt(collision_response.collision_count_non_resting), LogDebugLevel::Informational);

    for (auto i = 0; i < vehicle_api_messages_.size(); ++i) {
        UAirBlueprintLib::LogMessage(FString(vehicle_api_messages_[i].c_str()), TEXT(""), LogDebugLevel::Success, 30);
    }

    try {
        vehicle_api_->sendTelemetry(dt);
    }
    catch (std::exception &e) {
        UAirBlueprintLib::LogMessage(FString(e.what()), TEXT(""), LogDebugLevel::Failure, 30);
    }

    pawn_events_->getActuatorSignal().emit(rotor_info_);
}

void TiltrotorPawnSimApi::setPose(const Pose& pose, bool ignore_collision)
{
    pending_phys_pose_ = pose;
    pending_pose_collisions_ = ignore_collision;
    pending_pose_status_ = PendingPoseStatus::RenderStatePending;
}

//*** Start: UpdatableState implementation ***//
void TiltrotorPawnSimApi::resetImplementation()
{
    PawnSimApi::resetImplementation();

    vehicle_api_->reset();
    aero_physics_body_->reset();
    vehicle_api_messages_.clear();
}

//this is high frequency physics tick, flier gets ticked at rendering frame rate
void TiltrotorPawnSimApi::update()
{
    //environment update for current position
    PawnSimApi::update();

    //update forces on vertices
    aero_physics_body_->update();

    //update to controller must be done after kinematics have been updated by physics engine
}

void TiltrotorPawnSimApi::reportState(StateReporter& reporter)
{
    PawnSimApi::reportState(reporter);

    aero_physics_body_->reportState(reporter);
}

TiltrotorPawnSimApi::UpdatableObject* TiltrotorPawnSimApi::getPhysicsBody()
{
    return aero_physics_body_->getPhysicsBody();
}
//*** End: UpdatableState implementation ***//
