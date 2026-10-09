#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.h"
#include "TOHCharacter.generated.h"

UCLASS()
class TOH_API ATOHCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATOHCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FTOHHero HeroData;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Health = 100.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float MaxHealth = 100.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bIsBoss = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ActiveAbilityName = TEXT("None");

    UFUNCTION(BlueprintCallable)
    void SetHeroData(const FTOHHero& InHeroData);

    UFUNCTION(BlueprintCallable)
    void PrimaryAttack();

    UFUNCTION(BlueprintCallable)
    void SecondaryAttack();

    UFUNCTION(BlueprintCallable)
    void Ability1();

    UFUNCTION(BlueprintCallable)
    void Ability2();

    UFUNCTION(BlueprintCallable)
    void Ability3();

protected:
    void MoveForward(float Value);
    void MoveRight(float Value);
};
