#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hero.h"
#include "World.generated.h"

USTRUCT(BlueprintType)
struct FTOHLocation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) FString Id;
    UPROPERTY(BlueprintReadWrite) FString Name;
    UPROPERTY(BlueprintReadWrite) FString Description;
    UPROPERTY(BlueprintReadWrite) FVector LocationCoord;
    UPROPERTY(BlueprintReadWrite) FString Type;
    UPROPERTY(BlueprintReadWrite) FString ControlledBy;
    UPROPERTY(BlueprintReadWrite) bool bIsActive;
    UPROPERTY(BlueprintReadWrite) int32 Difficulty;
};

USTRUCT(BlueprintType)
struct FTOHMission
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) FString Id;
    UPROPERTY(BlueprintReadWrite) FString Title;
    UPROPERTY(BlueprintReadWrite) FString Description;
    UPROPERTY(BlueprintReadWrite) FString LocationId;
    UPROPERTY(BlueprintReadWrite) FString BossId;
    UPROPERTY(BlueprintReadWrite) int32 Reward;
    UPROPERTY(BlueprintReadWrite) bool bCompleted;
    UPROPERTY(BlueprintReadWrite) FString ObjectiveType;
};

USTRUCT(BlueprintType)
struct FTOHFacility
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) FString Name;
    UPROPERTY(BlueprintReadWrite) FString Sector;
    UPROPERTY(BlueprintReadWrite) FString Description;
    UPROPERTY(BlueprintReadWrite) TArray<FTOHLocation> Locations;
    UPROPERTY(BlueprintReadWrite) TArray<FTOHMission> Missions;
};

UCLASS()
class TOH_API AWorldManager : public AActor
{
    GENERATED_BODY()

public:
    AWorldManager();
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite) TArray<FTOHFacility> Facilities;
    UPROPERTY(BlueprintReadWrite) TArray<FTOHLocation> AllLocations;
    UPROPERTY(BlueprintReadWrite) TArray<FTOHMission> AllMissions;

    UFUNCTION(BlueprintCallable)
    void LoadWorld();

    UFUNCTION(BlueprintCallable)
    FTOHLocation GetLocationById(const FString& LocationId) const;

    UFUNCTION(BlueprintCallable)
    TArray<FTOHLocation> GetLocationsByDistrict(const FString& District) const;

    UFUNCTION(BlueprintCallable)
    FTOHMission GetMissionById(const FString& MissionId) const;

    UFUNCTION(BlueprintCallable)
    TArray<FTOHMission> GetAvailableMissions() const;
};
