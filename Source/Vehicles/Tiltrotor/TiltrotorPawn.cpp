#include "TiltrotorPawn.h"
#include "AirBlueprintLib.h"
#include "common/CommonStructs.hpp"
#include "common/Common.hpp"

ATiltrotorPawn::ATiltrotorPawn()
{
    pawn_events_.getActuatorSignal().connect_member(this, &ATiltrotorPawn::setRotorRenderedStates);
}

void ATiltrotorPawn::BeginPlay()
{
    Super::BeginPlay();

    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_L")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_R")));

    rotor_angle_components_.Add(UAirBlueprintLib::GetActorComponent<UStaticMeshComponent>(this, TEXT("Engine_L")));
    rotor_angle_components_.Add(UAirBlueprintLib::GetActorComponent<UStaticMeshComponent>(this, TEXT("Engine_R")));
}

void ATiltrotorPawn::initializeForBeginPlay()
{
    //get references of existing camera
    camera_front_right_ = Cast<APIPCamera>(
        (UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("FrontRightCamera")))->GetChildActor());
    camera_front_left_ = Cast<APIPCamera>(
        (UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("FrontLeftCamera")))->GetChildActor());
    camera_front_center_ = Cast<APIPCamera>(
        (UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("FrontCenterCamera")))->GetChildActor());
    camera_back_center_ = Cast<APIPCamera>(
        (UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("BackCenterCamera")))->GetChildActor());
    camera_bottom_center_ = Cast<APIPCamera>(
        (UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("BottomCenterCamera")))->GetChildActor());
}

void ATiltrotorPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    pawn_events_.getPawnTickSignal().emit(DeltaSeconds);
}


void ATiltrotorPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    camera_front_right_ = nullptr;
    camera_front_left_ = nullptr;
    camera_front_center_ = nullptr;
    camera_back_center_ = nullptr;
    camera_bottom_center_ = nullptr;

    pawn_events_.getActuatorSignal().disconnect_all();
    rotor_speed_components_.Empty();
    rotor_angle_components_.Empty();

    Super::EndPlay(EndPlayReason);
}

const common_utils::UniqueValueMap<std::string, APIPCamera*> ATiltrotorPawn::getCameras() const
{
    common_utils::UniqueValueMap<std::string, APIPCamera*> cameras;
    cameras.insert_or_assign("front_center", camera_front_center_);
    cameras.insert_or_assign("front_right", camera_front_right_);
    cameras.insert_or_assign("front_left", camera_front_left_);
    cameras.insert_or_assign("bottom_center", camera_bottom_center_);
    cameras.insert_or_assign("back_center", camera_back_center_);

    cameras.insert_or_assign("0", camera_front_center_);
    cameras.insert_or_assign("1", camera_front_right_);
    cameras.insert_or_assign("2", camera_front_left_);
    cameras.insert_or_assign("3", camera_bottom_center_);
    cameras.insert_or_assign("4", camera_back_center_);

    cameras.insert_or_assign("", camera_front_center_);
    cameras.insert_or_assign("fpv", camera_front_center_);

    return cameras;
}

void ATiltrotorPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
    FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    pawn_events_.getCollisionSignal().emit(MyComp, Other, OtherComp, bSelfMoved, HitLocation,
        HitNormal, NormalImpulse, Hit);
}

void ATiltrotorPawn::setRotorRenderedStates(const std::vector<TiltrotorPawnEvents::RotorTiltableInfo>& rotor_infos)
{
    // only iterate over num_visible_rotors_ since mesh doesn't have back rotor
    for (auto rotor_index = 0; rotor_index < num_visible_rotors_; ++rotor_index) {
        URotatingMovementComponent* rot_movement = rotor_speed_components_[rotor_index];
        if (rot_movement != nullptr) {
            rot_movement->RotationRate.Yaw =
                rotor_infos.at(rotor_index).rotor_speed * rotor_infos.at(rotor_index).rotor_direction *
                180.0f / M_PIf * RotatorFactor;
        }
        if (!rotor_infos.at(rotor_index).is_fixed) {
            UStaticMeshComponent* rotor_angle_comp = rotor_angle_components_[rotor_index];
            if (rotor_angle_comp != nullptr) {
                float pitch = -rotor_infos.at(rotor_index).rotor_angle_from_vertical * 180.0f / M_PIf * RotatorFactor;
                FRotator new_rotation = FRotator(pitch, 0.0f, 0.0f);
                rotor_angle_comp->SetRelativeRotation(new_rotation);
            }
        }
    }
}
