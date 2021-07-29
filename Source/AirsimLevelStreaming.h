#pragma once
#include "Runtime/Engine/Classes/Engine/LevelStreamingDynamic.h"
#include "Misc/PackageName.h"

class UAirsimLevelStreaming : public ULevelStreamingDynamic
{
public:
    static UAirsimLevelStreaming* LoadAirsimLevelInstance(UWorld* WorldContextObject, FString LevelName, FVector Location, FRotator Rotation, bool& bOutSuccess);

private:
    static int32 LevelInstanceId;
};