#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TOHCharacter.h"
#include "TOHTouchController.generated.h"

UCLASS()
class TOH_API ATOHTouchController : public APlayerController
{
    GENERATED_BODY()

public:
    ATOHTouchController();

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION()
    void OnTouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

    UFUNCTION()
    void OnTouchMoved(const ETouchIndex::Type FingerIndex, const FVector Location);

    UFUNCTION()
    void OnTouchEnded(const ETouchIndex::Type FingerIndex, const FVector Location);

protected:
    FVector TouchStartLocation;
    bool bTouchActive = false;

    UFUNCTION()
    void HandleTouchMovement(FVector TouchLocation);

    UFUNCTION()
    void HandleTouchAbility(FVector TouchLocation);
};
