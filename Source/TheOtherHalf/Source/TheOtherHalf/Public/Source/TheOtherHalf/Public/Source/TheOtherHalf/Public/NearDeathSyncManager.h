#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NearDeathSyncManager.generated.h"

UCLASS()
class THEOTHERHALF_API ANearDeathSyncManager : public AActor
{
  GENERATED_BODY()
public:
  UFUNCTION(Server, Reliable) void TryTriggerSync(float LonzoHP, float FatherInLawHP);
  UFUNCTION() void Revive_DisableCage_PowerDownWeapons(); // Final: foreground boss / background horde
};
