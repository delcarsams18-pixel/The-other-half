#include "TOHBossCharacter.h"

ATOHBossCharacter::ATOHBossCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ATOHBossCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (BossData.Name.Len() > 0)
    {
        Health = MaxHealth;
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
            FString::Printf(TEXT("%s loaded as boss: %s"), *BossData.Name, *BossData.Role));
    }
}

void ATOHBossCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATOHBossCharacter::SetBossData(const FTOHVillain& InBossData)
{
    BossData = InBossData;
    MaxHealth = 500.f;
    Health = MaxHealth;
}

void ATOHBossCharacter::BossAttack()
{
    if (BossData.Name.Len() == 0) return;

    GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Red,
        FString::Printf(TEXT("%s attacks with %s!"), *BossData.Name, *BossData.Trait));
}

void ATOHBossCharacter::BossAbility()
{
    if (BossData.Name.Len() == 0) return;

    GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Red,
        FString::Printf(TEXT("%s uses %s ability!"), *BossData.Name, *BossData.Trait));
}

void ATOHBossCharacter::MoveBossForward(float Value)
{
    if (Value == 0.f) return;
    AddMovementInput(GetActorForwardVector(), Value);
}

void ATOHBossCharacter::MoveBossRight(float Value)
{
    if (Value == 0.f) return;
    AddMovementInput(GetActorRightVector(), Value);
}
