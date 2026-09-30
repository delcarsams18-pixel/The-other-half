#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SplitBrainComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEOTHERHALF_API USplitBrainComponent : public UActorComponent
{
  GENERATED_BODY()
public:
  USplitBrainComponent(){}
  UPROPERTY(BlueprintReadOnly) float HeadSync = 50.f; // Half head caged - Warden/Overlord think they have her
  UPROPERTY(BlueprintReadOnly) float BaseSync = 50.f; // Half base secret same robotic
  UFUNCTION(Server, Reliable) void Server_SyncForRevive();
  UFUNCTION(Server, Reliable) void DisableCage_PowerDownWeapons();
};
