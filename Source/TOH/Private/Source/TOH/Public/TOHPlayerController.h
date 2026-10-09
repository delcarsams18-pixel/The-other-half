#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TOHPlayerController.generated.h"

UCLASS()
class TOH_API ATOHPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ATOHPlayerController();

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION()
    void MoveForward(float Value);

    UFUNCTION()
    void MoveRight(float Value);

    UFUNCTION()
    void PrimaryAttack();

    UFUNCTION()
    void SecondaryAttack();

    UFUNCTION()
    void Ability1();

    UFUNCTION()
    void Ability2();

    UFUNCTION()
    void Ability3();
};
