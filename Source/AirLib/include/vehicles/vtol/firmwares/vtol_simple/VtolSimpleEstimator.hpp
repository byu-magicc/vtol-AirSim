// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_VtolSimpleEstimator_hpp
#define msr_airlib_VtolSimpleEstimator_hpp

#include "firmware/interfaces/CommonStructs.hpp"
#include "VtolSimpleCommon.hpp"
#include "physics/Kinematics.hpp"
#include "physics/Environment.hpp"
#include "common/Common.hpp"

namespace msr
{
namespace airlib
{

    class VtolSimpleEstimator : public vtol_simple::IStateEstimator
    {
    public:
        virtual ~VtolSimpleEstimator() {}

        //for now we don't do any state estimation and use ground truth (i.e. assume perfect sensors)
        void setGroundTruthKinematics(const Kinematics::State* kinematics, const Environment* environment)
        {
            kinematics_ = kinematics;
            environment_ = environment;
        }

        virtual vtol_simple::Axis3r getAngles() const override
        {
            vtol_simple::Axis3r angles;
            VectorMath::toEulerianAngle(kinematics_->pose.orientation,
                                        angles.pitch(),
                                        angles.roll(),
                                        angles.yaw());

            //Utils::log(Utils::stringf("Ang Est:\t(%f, %f, %f)", angles.pitch(), angles.roll(), angles.yaw()));

            return angles;
        }

        virtual vtol_simple::Axis3r getAngularVelocity() const override
        {
            const auto& anguler = kinematics_->twist.angular;

            vtol_simple::Axis3r conv;
            conv.x() = anguler.x();
            conv.y() = anguler.y();
            conv.z() = anguler.z();

            return conv;
        }

        virtual vtol_simple::Axis3r getPosition() const override
        {
            return VtolSimpleCommon::toAxis3r(kinematics_->pose.position);
        }

        virtual vtol_simple::Axis3r transformToBodyFrame(const vtol_simple::Axis3r& world_frame_val) const override
        {
            const Vector3r& vec = VtolSimpleCommon::toVector3r(world_frame_val);
            const Vector3r& trans = VectorMath::transformToBodyFrame(vec, kinematics_->pose.orientation);
            return VtolSimpleCommon::toAxis3r(trans);
        }

        virtual vtol_simple::Axis3r getLinearVelocity() const override
        {
            return VtolSimpleCommon::toAxis3r(kinematics_->twist.linear);
        }

        virtual vtol_simple::Axis4r getOrientation() const override
        {
            return VtolSimpleCommon::toAxis4r(kinematics_->pose.orientation);
        }

        virtual vtol_simple::GeoPoint getGeoPoint() const override
        {
            return VtolSimpleCommon::toSimpleFlightGeoPoint(environment_->getState().geo_point);
        }

        virtual vtol_simple::GeoPoint getHomeGeoPoint() const override
        {
            return VtolSimpleCommon::toSimpleFlightGeoPoint(environment_->getHomeGeoPoint());
        }

        virtual vtol_simple::KinematicsState getKinematicsEstimated() const override
        {
            vtol_simple::KinematicsState state;
            state.position = getPosition();
            state.orientation = getOrientation();
            state.linear_velocity = getLinearVelocity();
            state.angular_velocity = getAngularVelocity();
            state.linear_acceleration = VtolSimpleCommon::toAxis3r(kinematics_->accelerations.linear);
            state.angular_acceleration = VtolSimpleCommon::toAxis3r(kinematics_->accelerations.angular);

            return state;
        }

    private:
        const Kinematics::State* kinematics_;
        const Environment* environment_;
    };

}
} //namespace
#endif
