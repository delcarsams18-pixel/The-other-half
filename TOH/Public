#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "Hero.generated.h"

USTRUCT(BlueprintType)
struct FTOHUnder
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) FString Level;
    UPROPERTY(BlueprintReadWrite) FString Name;
    UPROPERTY(BlueprintReadWrite) int32 HP = 100;
    UPROPERTY(BlueprintReadWrite) int32 DMG = 10;
    UPROPERTY(BlueprintReadWrite) FString Desc;
};

USTRUCT(BlueprintType)
struct FTOHHero
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) FString Id;
    UPROPERTY(BlueprintReadWrite) FString Name;
    UPROPERTY(BlueprintReadWrite) FString Role;
    UPROPERTY(BlueprintReadWrite) FString Tech;
    UPROPERTY(BlueprintReadWrite) FString A1;
    UPROPERTY(BlueprintReadWrite) FString A2;
    UPROPERTY(BlueprintReadWrite) FString A3;
    UPROPERTY(BlueprintReadWrite) FString Note;
    UPROPERTY(BlueprintReadWrite) TArray<FTOHUnder> Unders;
};

UCLASS()
class TOH_API AHero : public AActor
{
    GENERATED_BODY()
public:
    AHero();
    virtual void BeginPlay() override;
    UPROPERTY(BlueprintReadWrite) FTOHHero Data;
    UFUNCTION(BlueprintCallable) void Use1();
    UFUNCTION(BlueprintCallable) void Use2();
    UFUNCTION(BlueprintCallable) void Use3();
    UFUNCTION(BlueprintCallable) void Spawn(FString L);
};

UCLASS()
class TOH_API ATOHGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ATOHGameMode();
    UPROPERTY(BlueprintReadWrite) TArray<FTOHHero> AllHeroes;
    UFUNCTION(BlueprintCallable) void Load();
};