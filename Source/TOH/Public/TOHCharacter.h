#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProceduralMeshComponent.h"
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

    UPROPERTY(VisibleAnywhere, Category = "TOH|Visual")
    class UStaticMeshComponent* GunBody;

    UPROPERTY(VisibleAnywhere, Category = "TOH|Visual")
    class UStaticMeshComponent* GunBarrel;

    UPROPERTY(VisibleAnywhere, Category = "TOH|Visual")
    class UStaticMeshComponent* GunGrip;

    UPROPERTY(VisibleAnywhere, Category = "TOH|Visual")
    class UStaticMeshComponent* BodyMesh;

    UPROPERTY(VisibleAnywhere, Category = "TOH|Visual")
    class UProceduralMeshComponent* LonzoModel;

    UPROPERTY(VisibleAnywhere, Category = "TOH|Visual")
    class UProceduralMeshComponent* RifleModel;

    // Skeletal mesh (animated) - replaces procedural if loads successfully
    UPROPERTY(VisibleAnywhere, Category = "TOH|Animation")
    class USkeletalMeshComponent* SkelMesh;

    // Hard reference to ensure cooker includes the asset
    UPROPERTY(EditDefaultsOnly, Category = "TOH|Animation")
    class USkeletalMesh* LonzoSkelMesh;

    UPROPERTY(EditAnywhere, Category = "TOH|Animation")
    class UAnimSequence* IdleAnim;

    UPROPERTY(EditAnywhere, Category = "TOH|Animation")
    class UAnimSequence* WalkAnim;

    UPROPERTY(EditAnywhere, Category = "TOH|Animation")
    class UAnimSequence* RunAnim;

    UPROPERTY(VisibleAnywhere, Category = "TOH|Animation")
    bool bUseSkeletal = false;
    int32 WalkFrameIndex = 0;
    float WalkAnimTimer = 0.0f;
    // Stored mesh topology for walk animation updates
    TArray<int32> LonzoTriangles;
    TArray<FVector> LonzoNormals;
    TArray<FVector2D> LonzoUVs;
    TArray<FLinearColor> LonzoColors;
    TArray<FProcMeshTangent> LonzoTangents;

    UFUNCTION()
    void OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);

    UFUNCTION()
    void OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);

    FVector TouchStart = FVector::ZeroVector;
    bool bTouchActive = false;
    ETouchIndex::Type TouchFinger = ETouchIndex::Touch1;
};
