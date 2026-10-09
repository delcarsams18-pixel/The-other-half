#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TOH")
    float Health = 100.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TOH")
    float MaxHealth = 100.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TOH")
    float MoveSpeed = 600.0f;

    UFUNCTION(BlueprintCallable, Category = "TOH")
    void ApplyDamage(float Amount);

    UFUNCTION(BlueprintCallable, Category = "TOH")
    bool IsAlive() const { return Health > 0.0f; }

    UFUNCTION(BlueprintCallable, Category = "TOH")
    void Fire();

protected:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void Turn(float Value);

    UPROPERTY(VisibleAnywhere, Category = "TOH")
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, Category = "TOH")
    class UCameraComponent* FollowCamera;

    UPROPERTY(EditDefaultsOnly, Category = "TOH|Combat")
    TSubclassOf<class ATOHProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "TOH|Combat")
    float FireCooldown = 0.25f;

    float LastFireTime = -10.0f;

    UPROPERTY(VisibleAnywhere, Category = "TOH|Visual")
    class UStaticMeshComponent* ArmCannon;

    UFUNCTION()
    void OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);

    UFUNCTION()
    void OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);

    FVector TouchStart = FVector::ZeroVector;
    bool bTouchActive = false;
    ETouchIndex::Type TouchFinger = ETouchIndex::Touch1;
};
