#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TOHEnemy.generated.h"

UCLASS()
class TOH_API ATOHEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    ATOHEnemy();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TOH")
    float Health = 50.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TOH")
    float MaxHealth = 50.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TOH")
    float Damage = 10.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TOH")
    float AttackRange = 150.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TOH")
    float SightRange = 2500.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TOH")
    float AttackCooldown = 1.2f;

    UFUNCTION(BlueprintCallable, Category = "TOH")
    void ApplyDamage(float Amount);

    UFUNCTION(BlueprintCallable, Category = "TOH")
    bool IsAlive() const { return Health > 0.0f; }

protected:
    UPROPERTY(VisibleAnywhere, Category = "TOH")
    class UStaticMeshComponent* BodyMesh;

    UPROPERTY(EditDefaultsOnly, Category = "TOH")
    TSubclassOf<class ATOHProjectile> ProjectileClass;

    float LastAttackTime = -10.0f;
    class ATOHCharacter* TargetPlayer = nullptr;

    void Attack();
};
