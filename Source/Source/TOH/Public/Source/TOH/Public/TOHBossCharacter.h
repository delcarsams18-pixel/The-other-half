#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.h"
#include "TOHCharacter.h"
#include "TOHBossCharacter.generated.h"

UCLASS()
class TOH_API ATOHBossCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATOHBossCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FTOHVillain BossData;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Health = 500.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float MaxHealth = 500.f;

    UFUNCTION(BlueprintCallable)
    void SetBossData(const FTOHVillain& InBossData);

    UFUNCTION(BlueprintCallable)
    void BossAttack();

    UFUNCTION(BlueprintCallable)
    void BossAbility();

protected:
    void MoveBossForward(float Value);
    void MoveBossRight(float Value);
};
