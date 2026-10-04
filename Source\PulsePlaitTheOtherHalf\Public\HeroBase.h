#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DistrictData.h"
#include "HeroBase.generated.h"
UCLASS() class PULSEPLAITTOTHERHALF_API AHeroBase : public ACharacter { GENERATED_BODY() public: AHeroBase(); UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pulse Plait | The Other Half") FHeroData HeroData; UFUNCTION(BlueprintCallable) void UseAbility1(); UFUNCTION(BlueprintCallable) void UseAbility2(); UFUNCTION(BlueprintCallable) void UseAbility3(); UFUNCTION(BlueprintCallable) void SpawnUnderling(FString Level); protected: virtual void BeginPlay() override; };