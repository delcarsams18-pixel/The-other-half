#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HeroData.h"
#include "TOHGameMode.generated.h"

UCLASS()
class TOH_API ATOHGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ATOHGameMode();
    UPROPERTY(EditAnywhere,BlueprintReadWrite) TArray<FTOHHero> AllHeroes;
    UFUNCTION(BlueprintCallable) void Load();
};