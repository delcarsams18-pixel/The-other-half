#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Hero.h"
#include "TOHGameMode.generated.h"
UCLASS() class TOH_API ATOHGameMode : public AGameModeBase { GENERATED_BODY() public: ATOHGameMode(); UPROPERTY(EditAnywhere,BlueprintReadWrite) TArray<FHero> AllHeroes; UFUNCTION(BlueprintCallable) void Load(); };