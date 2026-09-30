#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WirelessUpgradeComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEOTHERHALF_API UWirelessUpgradeComponent : public UActorComponent
{
  GENERATED_BODY()
public:
  UPROPERTY(BlueprintReadOnly) float Progress = 0.f;
  UPROPERTY(BlueprintReadOnly) bool bCanTrack = false;
  UPROPERTY(BlueprintReadOnly) bool bCanAssist = false;
  UFUNCTION(BlueprintCallable) void AddTaskProgress(float Val=2.f){
    Progress=FMath::Clamp(Progress+Val,0,100);
    if(Progress>=75) bCanTrack=true;
    if(Progress>=90) bCanAssist=true;
  }
};
