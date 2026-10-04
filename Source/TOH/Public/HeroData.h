#pragma once
#include "CoreMinimal.h"
#include "HeroData.generated.h"

USTRUCT(BlueprintType)
struct FUnder
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) FString Level;
    UPROPERTY(BlueprintReadWrite) FString Name;
    UPROPERTY(BlueprintReadWrite) int32 HP=100;
    UPROPERTY(BlueprintReadWrite) int32 DMG=20;
    UPROPERTY(BlueprintReadWrite) FString Desc;
};

USTRUCT(BlueprintType)
struct FHero
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
    UPROPERTY(BlueprintReadWrite) TArray<FUnder> Unders;
};