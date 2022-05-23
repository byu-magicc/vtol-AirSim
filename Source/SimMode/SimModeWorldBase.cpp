#include "SimModeWorldBase.h"
#include "physics/FastPhysicsEngine.hpp"
#include "physics/ExternalPhysicsEngine.hpp"
#include "common/AirSimSettings.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include "AirBlueprintLib.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Engine/World.h"
#include "UObject/Class.h"
#include "Materials/MaterialInstanceDynamic.h"


void ASimModeWorldBase::BeginPlay()
{
    Super::BeginPlay();

    // Spawns in the guidance lights based on the settings.json file.
    // Lights are defined in settings.json with the following format:
        // "GuidanceLights": {
        //     "LandingLocation": {"X": 100.0, "Y": 100.0, "Z": 100.0},
        //     "LightLocations": [
        //     {"Color": "1", "X": 0, "Y": 0.0, "Z": 100.0},
        //     {"Color": "2", "X": 0, "Y": 0.0, "Z": 150.0},
        //     {"Color": "3", "X": 0.0, "Y": 0.0, "Z": 200.0}
        //     ]
        // }
    // All coordinates are in the local coordinate frame of the PlayerStart object in Unreal Engine. Meaning,
    // a landing location of (0, 0, 0) will set it to the same position as the PlayerStart. Not sure how to
    // switch this to global.

    AirSimSettings::GuidanceLightSetting& landing_location = AirSimSettings::singleton().landing_location;
    const std::vector<AirSimSettings::GuidanceLightSetting>& light_locations = AirSimSettings::singleton().light_locations;

    FRotator spawn_rotation;
    FActorSpawnParameters spawn_params;

    for (auto current_light : light_locations) {
        UClass* bp_class;
        if (current_light.light_color == "1") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_0.BP_Light_0_C'");
        } else if (current_light.light_color == "2") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_6.BP_Light_6_C'");
        } else if (current_light.light_color == "3") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_12.BP_Light_12_C'");
        } else if (current_light.light_color == "4") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_18.BP_Light_18_C'");
        } else if (current_light.light_color == "5") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_24.BP_Light_24_C'");
        } else if (current_light.light_color == "6") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_30.BP_Light_30_C'");
        } else if (current_light.light_color == "7") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_36.BP_Light_36_C'");
        } else if (current_light.light_color == "8") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_42.BP_Light_42_C'");
        } else if (current_light.light_color == "9") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_48.BP_Light_48_C'");
        } else if (current_light.light_color == "10") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_54.BP_Light_54_C'");
        } else if (current_light.light_color == "11") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_60.BP_Light_60_C'");
        } else if (current_light.light_color == "12") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_66.BP_Light_66_C'");
        } else if (current_light.light_color == "13") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_72.BP_Light_72_C'");
        } else if (current_light.light_color == "14") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_78.BP_Light_78_C'");
        } else if (current_light.light_color == "15") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_84.BP_Light_84_C'");
        } else if (current_light.light_color == "16") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_90.BP_Light_90_C'");
        } else if (current_light.light_color == "17") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_96.BP_Light_96_C'");
        } else if (current_light.light_color == "18") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_102.BP_Light_102_C'");
        } else if (current_light.light_color == "19") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_108.BP_Light_108_C'");
        } else if (current_light.light_color == "20") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_114.BP_Light_114_C'");
        } else if (current_light.light_color == "21") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_120.BP_Light_120_C'");
        } else if (current_light.light_color == "22") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_126.BP_Light_126_C'");
        } else if (current_light.light_color == "23") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_132.BP_Light_132_C'");
        } else if (current_light.light_color == "24") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_138.BP_Light_138_C'");
        } else if (current_light.light_color == "25") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_144.BP_Light_144_C'");
        } else if (current_light.light_color == "26") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_150.BP_Light_150_C'");
        } else if (current_light.light_color == "27") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_156.BP_Light_156_C'");
        } else if (current_light.light_color == "28") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_162.BP_Light_162_C'");
        } else if (current_light.light_color == "29") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_168.BP_Light_168_C'");
        } else if (current_light.light_color == "30") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_174.BP_Light_174_C'");
        }
        else if (current_light.light_color == "31") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_180.BP_Light_180_C'");
        } else if (current_light.light_color == "32") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_186.BP_Light_186_C'");
        } else if (current_light.light_color == "33") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_192.BP_Light_192_C'");
        } else if (current_light.light_color == "34") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_198.BP_Light_198_C'");
        } else if (current_light.light_color == "35") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_204.BP_Light_204_C'");
        } else if (current_light.light_color == "36") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_210.BP_Light_210_C'");
        } else if (current_light.light_color == "37") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_216.BP_Light_216_C'");
        } else if (current_light.light_color == "38") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_222.BP_Light_222_C'");
        } else if (current_light.light_color == "39") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_228.BP_Light_228_C'");
        } else if (current_light.light_color == "40") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_234.BP_Light_234_C'");
        } else if (current_light.light_color == "41") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_240.BP_Light_240_C'");
        } else if (current_light.light_color == "42") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_246.BP_Light_246_C'");
        } else if (current_light.light_color == "43") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_252.BP_Light_252_C'");
        } else if (current_light.light_color == "44") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_258.BP_Light_258_C'");
        } else if (current_light.light_color == "45") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_264.BP_Light_264_C'");
        } else if (current_light.light_color == "46") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_270.BP_Light_270_C'");
        } else if (current_light.light_color == "47") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_276.BP_Light_276_C'");
        } else if (current_light.light_color == "48") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_282.BP_Light_282_C'");
        } else if (current_light.light_color == "49") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_288.BP_Light_288_C'");
        } else if (current_light.light_color == "50") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_294.BP_Light_294_C'");
        } else if (current_light.light_color == "51") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_300.BP_Light_300_C'");
        } else if (current_light.light_color == "52") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_306.BP_Light_306_C'");
        } else if (current_light.light_color == "53") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_312.BP_Light_312_C'");
        } else if (current_light.light_color == "54") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_318.BP_Light_318_C'");
        } else if (current_light.light_color == "55") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_324.BP_Light_324_C'");
        } else if (current_light.light_color == "56") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_330.BP_Light_330_C'");
        } else if (current_light.light_color == "57") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_336.BP_Light_336_C'");
        } else if (current_light.light_color == "58") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_342.BP_Light_342_C'");
        } else if (current_light.light_color == "59") {
            bp_class = UAirBlueprintLib::LoadClass("Class'/AirSim/GuidanceLights/BP_Light_348.BP_Light_348_C'");
        } else {
            // TODO: Add proper error report
            std::cout << "ERROR: Incorrect color ID for GuidanceLights in settings.json" << std::endl;
            continue;
        }

        double x_location = current_light.x_location + landing_location.x_location;
        double y_location = current_light.y_location + landing_location.y_location;
        double z_location = current_light.z_location + landing_location.z_location;

        FVector spawn_position = FVector(x_location, y_location, z_location);

        APawn* spawned_pawn = static_cast<APawn*>(this->GetWorld()->SpawnActor(
            bp_class, &spawn_position, &spawn_rotation, spawn_params));
    }
}

void ASimModeWorldBase::initializeForPlay()
{
    std::vector<msr::airlib::UpdatableObject*> vehicles;
    for (auto& api : getApiProvider()->getVehicleSimApis())
        vehicles.push_back(api);
    //TODO: directly accept getVehicleSimApis() using generic container

    std::unique_ptr<PhysicsEngineBase> physics_engine = createPhysicsEngine();
    physics_engine_ = physics_engine.get();
    physics_world_.reset(new msr::airlib::PhysicsWorld(std::move(physics_engine),
                                                       vehicles,
                                                       getPhysicsLoopPeriod()));
}

void ASimModeWorldBase::registerPhysicsBody(msr::airlib::VehicleSimApiBase* physicsBody)
{
    physics_world_.get()->addBody(physicsBody);
}

void ASimModeWorldBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    //remove everything that we created in BeginPlay
    physics_world_.reset();

    Super::EndPlay(EndPlayReason);
}

void ASimModeWorldBase::startAsyncUpdator()
{
    physics_world_->startAsyncUpdator();
}

void ASimModeWorldBase::stopAsyncUpdator()
{
    physics_world_->stopAsyncUpdator();
}

long long ASimModeWorldBase::getPhysicsLoopPeriod() const //nanoseconds
{
    return physics_loop_period_;
}
void ASimModeWorldBase::setPhysicsLoopPeriod(long long period)
{
    physics_loop_period_ = period;
}

std::unique_ptr<ASimModeWorldBase::PhysicsEngineBase> ASimModeWorldBase::createPhysicsEngine()
{
    std::unique_ptr<PhysicsEngineBase> physics_engine;
    std::string physics_engine_name = getSettings().physics_engine_name;
    if (physics_engine_name == "")
        physics_engine.reset(); //no physics engine
    else if (physics_engine_name == "FastPhysicsEngine") {
        msr::airlib::Settings fast_phys_settings;
        if (msr::airlib::Settings::singleton().getChild("FastPhysicsEngine", fast_phys_settings)) {
            physics_engine.reset(new msr::airlib::FastPhysicsEngine(fast_phys_settings.getBool("EnableGroundLock", true)));
        }
        else {
            physics_engine.reset(new msr::airlib::FastPhysicsEngine());
        }

        physics_engine->setWind(getSettings().wind);
    }
    else if (physics_engine_name == "ExternalPhysicsEngine") {
        physics_engine.reset(new msr::airlib::ExternalPhysicsEngine());
    }
    else {
        physics_engine.reset();
        UAirBlueprintLib::LogMessageString("Unrecognized physics engine name: ", physics_engine_name, LogDebugLevel::Failure);
    }

    return physics_engine;
}

bool ASimModeWorldBase::isPaused() const
{
    return physics_world_->isPaused();
}

void ASimModeWorldBase::pause(bool is_paused)
{
    physics_world_->pause(is_paused);
    UGameplayStatics::SetGamePaused(this->GetWorld(), is_paused);
}

void ASimModeWorldBase::continueForTime(double seconds)
{
    int64 start_frame_number = UKismetSystemLibrary::GetFrameCount();
    if (physics_world_->isPaused()) {
        physics_world_->pause(false);
        UGameplayStatics::SetGamePaused(this->GetWorld(), false);
    }

    physics_world_->continueForTime(seconds);
    while (!physics_world_->isPaused()) {
        continue;
    }
    // wait if no new frame is rendered
    while (start_frame_number == UKismetSystemLibrary::GetFrameCount()) {
        continue;
    }
    UGameplayStatics::SetGamePaused(this->GetWorld(), true);
}

void ASimModeWorldBase::continueForFrames(uint32_t frames)
{
    if (physics_world_->isPaused()) {
        physics_world_->pause(false);
        UGameplayStatics::SetGamePaused(this->GetWorld(), false);
    }

    physics_world_->setFrameNumber((uint32_t)GFrameNumber);
    physics_world_->continueForFrames(frames);
    while (!physics_world_->isPaused()) {
        physics_world_->setFrameNumber((uint32_t)GFrameNumber);
    }
    UGameplayStatics::SetGamePaused(this->GetWorld(), true);
}

void ASimModeWorldBase::setWind(const msr::airlib::Vector3r& wind) const
{
    physics_engine_->setWind(wind);
}

void ASimModeWorldBase::updateDebugReport(msr::airlib::StateReporterWrapper& debug_reporter)
{
    unused(debug_reporter);
    //we use custom debug reporting for this class
}

void ASimModeWorldBase::Tick(float DeltaSeconds)
{
    { //keep this lock as short as possible
        physics_world_->lock();

        physics_world_->enableStateReport(EnableReport);
        physics_world_->updateStateReport();

        for (auto& api : getApiProvider()->getVehicleSimApis())
            api->updateRenderedState(DeltaSeconds);

        physics_world_->unlock();
    }

    //perform any expensive rendering update outside of lock region
    for (auto& api : getApiProvider()->getVehicleSimApis())
        api->updateRendering(DeltaSeconds);

    Super::Tick(DeltaSeconds);
}

void ASimModeWorldBase::reset()
{
    UAirBlueprintLib::RunCommandOnGameThread([this]() {
        physics_world_->reset();
    },
                                             true);

    //no need to call base reset because of our custom implementation
}

std::string ASimModeWorldBase::getDebugReport()
{
    return physics_world_->getDebugReport();
}
