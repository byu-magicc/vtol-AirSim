// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "SimModeWorldMaker.h"
#include "UObject/ConstructorHelpers.h"
#include "Logging/MessageLog.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#include "AirBlueprintLib.h"
#include "vehicles/tiltrotor/api/TiltrotorApiBase.hpp"
#include "MakerPawnSimApi.h"
#include "physics/PhysicsBody.hpp"
#include "common/ClockFactory.hpp"
#include <memory>
#include "vehicles/tiltrotor/api/TiltrotorRpcLibServer.hpp"
#include "common/SteppableClock.hpp"

void ASimModeWorldMaker::BeginPlay()
{
    Super::BeginPlay();

    //let base class setup physics world
    initializeForPlay();
}

void ASimModeWorldMaker::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    //stop physics thread before we dismantle
    stopAsyncUpdator();

    Super::EndPlay(EndPlayReason);
}

void ASimModeWorldMaker::setupClockSpeed()
{
    typedef msr::airlib::ClockFactory ClockFactory;

    float clock_speed = getSettings().clock_speed;

    //setup clock in ClockFactory
    std::string clock_type = getSettings().clock_type;

    if (clock_type == "ScalableClock") {
        //scalable clock returns interval same as wall clock but multiplied by a scale factor
        ClockFactory::get(std::make_shared<msr::airlib::ScalableClock>(clock_speed == 1 ? 1 : 1 / clock_speed));
    }
    else if (clock_type == "SteppableClock") {
        //steppable clock returns interval that is a constant number irrespective of wall clock
        //we can either multiply this fixed interval by scale factor to speed up/down the clock
        //but that would cause vehicles like quadrotors to become unstable
        //so alternative we use here is instead to scale control loop frequency. The downside is that
        //depending on compute power available, we will max out control loop frequency and therefore can no longer
        //get increase in clock speed

        //Approach 1: scale clock period, no longer used now due to quadrotor instability
        //ClockFactory::get(std::make_shared<msr::airlib::SteppableClock>(
        //static_cast<msr::airlib::TTimeDelta>(getPhysicsLoopPeriod() * 1E-9 * clock_speed)));

        //Approach 2: scale control loop frequency if clock is speeded up
        if (clock_speed >= 1) {
            ClockFactory::get(std::make_shared<msr::airlib::SteppableClock>(
                static_cast<msr::airlib::TTimeDelta>(getPhysicsLoopPeriod() * 1E-9))); //no clock_speed multiplier

            setPhysicsLoopPeriod(getPhysicsLoopPeriod() / static_cast<long long>(clock_speed));
        }
        else {
            //for slowing down, this don't generate instability
            ClockFactory::get(std::make_shared<msr::airlib::SteppableClock>(
                static_cast<msr::airlib::TTimeDelta>(getPhysicsLoopPeriod() * 1E-9 * clock_speed)));
        }
    }
    else
        throw std::invalid_argument(common_utils::Utils::stringf(
            "clock_type %s is not recognized", clock_type.c_str()));
}

//-------------------------------- overrides -----------------------------------------------//

std::unique_ptr<msr::airlib::ApiServerBase> ASimModeWorldMaker::createApiServer() const
{
#ifdef AIRLIB_NO_RPC
    return ASimModeBase::createApiServer();
#else
    return std::unique_ptr<msr::airlib::ApiServerBase>(new msr::airlib::TiltrotorRpcLibServer(
        getApiProvider(), getSettings().api_server_address, getSettings().api_port));
#endif
}

void ASimModeWorldMaker::getExistingVehiclePawns(TArray<AActor*>& pawns) const
{
    UAirBlueprintLib::FindAllActor<TVehiclePawn>(this, pawns);
}

bool ASimModeWorldMaker::isVehicleTypeSupported(const std::string& vehicle_type) const
{
    return ((vehicle_type == AirSimSettings::kVehicleTypeMakerSimple) ||
            (vehicle_type == AirSimSettings::kVehicleTypePX4Tiltrotor));
}

std::string ASimModeWorldMaker::getVehiclePawnPathName(const AirSimSettings::VehicleSetting& vehicle_setting) const
{
    //decide which derived BP to use
    std::string pawn_path = vehicle_setting.pawn_path;
    if (pawn_path == "")
        pawn_path = "DefaultMaker";

    return pawn_path;
}

PawnEvents* ASimModeWorldMaker::getVehiclePawnEvents(APawn* pawn) const
{
    return static_cast<TVehiclePawn*>(pawn)->getPawnEvents();
}
const common_utils::UniqueValueMap<std::string, APIPCamera*> ASimModeWorldMaker::getVehiclePawnCameras(
    APawn* pawn) const
{
    return (static_cast<const TVehiclePawn*>(pawn))->getCameras();
}
void ASimModeWorldMaker::initializeVehiclePawn(APawn* pawn)
{
    static_cast<TVehiclePawn*>(pawn)->initializeForBeginPlay();
}
std::unique_ptr<PawnSimApi> ASimModeWorldMaker::createVehicleSimApi(
    const PawnSimApi::Params& pawn_sim_api_params) const
{
    auto vehicle_sim_api = std::unique_ptr<PawnSimApi>(new MakerPawnSimApi(pawn_sim_api_params));
    vehicle_sim_api->initialize();
    //For multirotors the vehicle_sim_api are in PhysicsWOrld container and then get reseted when world gets reseted
    //vehicle_sim_api->reset();
    return vehicle_sim_api;
}
msr::airlib::VehicleApiBase* ASimModeWorldMaker::getVehicleApi(const PawnSimApi::Params& pawn_sim_api_params,
                                                                   const PawnSimApi* sim_api) const
{
    const auto maker_sim_api = static_cast<const MakerPawnSimApi*>(sim_api);
    return maker_sim_api->getVehicleApi();
}