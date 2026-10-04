#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeroData.h"
#include "Hero.generated.h"

UCLASS()
class TOH_API AHero : public ACharacter
{
    GENERATED_BODY()
public:
    AHero();
    UPROPERTY(EditAnywhere,BlueprintReadWrite) FTOHHero Data;
    UFUNCTION(BlueprintCallable) void Use1();
    UFUNCTION(BlueprintCallable) void Use2();
    UFUNCTION(BlueprintCallable) void Use3();
    UFUNCTION(BlueprintCallable) void Spawn(FString L);
protected: virtual void BeginPlay() override;
};