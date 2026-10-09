#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TOHProjectile.generated.h"

UCLASS()
class TOH_API ATOHProjectile : public AActor
{
    GENERATED_BODY()

public:
    ATOHProjectile();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, Category = "TOH")
    float Damage = 25.0f;

    UPROPERTY(EditDefaultsOnly, Category = "TOH")
    float Speed = 2500.0f;

    UPROPERTY(EditDefaultsOnly, Category = "TOH")
    float LifeTime = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category = "TOH")
    bool bIsEnemyProjectile = false;

protected:
    UPROPERTY(VisibleAnywhere, Category = "TOH")
    class USphereComponent* Collision;

    UPROPERTY(VisibleAnywhere, Category = "TOH")
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, Category = "TOH")
    class UProjectileMovementComponent* MoveComp;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        FVector NormalImpulse, const FHitResult& Hit);

    float Age = 0.0f;
};
