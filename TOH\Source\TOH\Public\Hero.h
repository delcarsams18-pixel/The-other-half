#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

USTRUCT(BlueprintType)
struct FUnder
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) FString Level; // T1 T2 T3
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

UCLASS()
class TOH_API AHero : public ACharacter
{
    GENERATED_BODY()
public:
    AHero();
    UPROPERTY(EditAnywhere,BlueprintReadWrite) FHero Data;
    UFUNCTION(BlueprintCallable) void Use1();
    UFUNCTION(BlueprintCallable) void Use2();
    UFUNCTION(BlueprintCallable) void Use3();
    UFUNCTION(BlueprintCallable) void Spawn(FString L);
protected: virtual void BeginPlay() override;
};