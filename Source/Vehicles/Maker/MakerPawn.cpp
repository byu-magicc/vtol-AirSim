#include "MakerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "AirBlueprintLib.h"
#include "common/CommonStructs.hpp"
#include "common/Common.hpp"

AMakerPawn::AMakerPawn()
{
    pawn_events_.getActuatorSignal().connect_member(this, &AMakerPawn::setRotorRenderedStates);
}

void AMakerPawn::BeginPlay()
{
    Super::BeginPlay();

    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_L1")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_L2")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_L3")));

    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_R1")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_R2")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_R3")));

    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_Back1")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_Back2")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_Back3")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_Back4")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_Back5")));
    rotor_speed_components_.Add(UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation_Back6")));

    rotor_angle_components_.Add(UAirBlueprintLib::GetActorComponent<UStaticMeshComponent>(this, TEXT("MotorPivot1")));
    rotor_angle_components_.Add(UAirBlueprintLib::GetActorComponent<UStaticMeshComponent>(this, TEXT("MotorPivot2")));
    rotor_angle_components_.Add(UAirBlueprintLib::GetActorComponent<UStaticMeshComponent>(this, TEXT("MotorPivot3")));
    rotor_angle_components_.Add(UAirBlueprintLib::GetActorComponent<UStaticMeshComponent>(this, TEXT("MotorPivot4")));
    rotor_angle_components_.Add(UAirBlueprintLib::GetActorComponent<UStaticMeshComponent>(this, TEXT("MotorPivot5")));
    rotor_angle_components_.Add(UAirBlueprintLib::GetActorComponent<UStaticMeshComponent>(this, TEXT("MotorPivot6")));

    int num_motors = 6;
    for (int i = 0; i < num_motors; ++i) 
    {
        ini_rotor_angles.push_back(rotor_angle_components_[i]->GetRelativeRotation());
    }
}

void AMakerPawn::initializeForBeginPlay()
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

void AMakerPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    pawn_events_.getPawnTickSignal().emit(DeltaSeconds);
}

void AMakerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

const common_utils::UniqueValueMap<std::string, APIPCamera*> AMakerPawn::getCameras() const
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

void AMakerPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
                               FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    pawn_events_.getCollisionSignal().emit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void AMakerPawn::setRotorRenderedStates(const std::vector<MakerPawnEvents::RotorTiltableInfo>& rotor_infos)
{
    // only iterate over num of motors coming from convergence aircraft (current airlib physics VTOL)
    for (auto rotor_index = 0; rotor_index < rotor_infos.size(); ++rotor_index) {
        switch (rotor_index){
            case 0: // front left propellors
                for (auto prop_index = rotor_index; prop_index < 3 ; ++prop_index){
                    URotatingMovementComponent* rot_movement = rotor_speed_components_[prop_index];
                    if (rot_movement != nullptr) {
                        if (prop_index != 1){
                            rot_movement->RotationRate.Yaw =
                                rotor_infos.at(rotor_index).rotor_speed * rotor_infos.at(rotor_index).rotor_direction * RotatorFactor;
                        }
                        else{
                            rot_movement->RotationRate.Yaw =
                                -rotor_infos.at(rotor_index).rotor_speed * rotor_infos.at(rotor_index).rotor_direction * RotatorFactor;
                        }
                    }        
                }
                break;
            case 1: // front right propellors
                for (auto prop_index = rotor_index * 3; prop_index < 6 ; ++prop_index){
                    URotatingMovementComponent* rot_movement = rotor_speed_components_[prop_index];
                    if (rot_movement != nullptr) {
                        if (prop_index != 4){
                            rot_movement->RotationRate.Yaw =
                                rotor_infos.at(rotor_index).rotor_speed * rotor_infos.at(rotor_index).rotor_direction * RotatorFactor;
                        }
                        else{
                            rot_movement->RotationRate.Yaw =
                                -rotor_infos.at(rotor_index).rotor_speed * rotor_infos.at(rotor_index).rotor_direction * RotatorFactor;
                        }
                    }
                }
                break;
            
            case 2: // back propellors
                
                for (auto prop_index = rotor_index * 3; prop_index < 12 ; ++prop_index){
                    URotatingMovementComponent* rot_movement = rotor_speed_components_[prop_index];
                    if (rot_movement != nullptr) {
                        if (prop_index % 2 != 0){
                            rot_movement->RotationRate.Yaw =
                                rotor_infos.at(rotor_index).rotor_speed * rotor_infos.at(rotor_index).rotor_direction * RotatorFactor;
                        }
                        else{
                            rot_movement->RotationRate.Yaw =
                                -rotor_infos.at(rotor_index).rotor_speed * rotor_infos.at(rotor_index).rotor_direction * RotatorFactor;
                        }
                    }
                }
                break;
        }
        
        if (!rotor_infos.at(rotor_index).is_fixed) {
            int max_index = rotor_angle_components_.Num() / 2 * (rotor_index + 1);     // max_index=3 for left, max_index=6 for right
            for (auto motor_index = 3 * rotor_index; motor_index <  max_index; ++motor_index){ // starting index is 0 for left, 3 for right.
                UStaticMeshComponent* rotor_angle_comp = rotor_angle_components_[motor_index];
                if (rotor_angle_comp != nullptr) {
                    float variable_angle = -rotor_infos.at(rotor_index).rotor_angle_from_vertical * 180.0f / M_PIf; // negate for unreal axes
                    FRotator new_rotation = FRotator(ini_rotor_angles.at(motor_index).Pitch, ini_rotor_angles.at(motor_index).Yaw, variable_angle);
                    rotor_angle_comp->SetRelativeRotation(new_rotation);
                }
            }
        }
    }
}
