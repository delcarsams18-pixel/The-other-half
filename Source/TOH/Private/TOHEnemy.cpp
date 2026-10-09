#include "TOHEnemy.h"
#include "TOHCharacter.h"
#include "TOHProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

ATOHEnemy::ATOHEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->MaxWalkSpeed = 350.0f;

    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (BodyMeshAsset.Succeeded())
    {
        BodyMesh->SetStaticMesh(BodyMeshAsset.Object);
        BodyMesh->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.8f));
        BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
    }
    BodyMesh->SetupAttachment(GetMesh());

    ProjectileClass = ATOHProjectile::StaticClass();
}

void ATOHEnemy::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;

    UMaterialInstanceDynamic* Mat = BodyMesh->CreateDynamicMaterialInstance(0);
    if (Mat)
    {
        Mat->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(0.15f, 0.02f, 0.02f));
        Mat->SetVectorParameterValue(TEXT("EmissiveColor"), FLinearColor(1.0f, 0.1f, 0.05f));
        Mat->SetScalarParameterValue(TEXT("EmissiveIntensity"), 2.0f);
    }

    TargetPlayer = Cast<ATOHCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ATOHEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!IsAlive() || !TargetPlayer || !TargetPlayer->IsAlive()) return;

    float Dist = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

    if (Dist > AttackRange && Dist < SightRange)
    {
        FVector Dir = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        AddMovementInput(Dir, 1.0f);
        FRotator LookRot = Dir.Rotation();
        SetActorRotation(FRotator(0.0f, LookRot.Yaw, 0.0f));
    }
    else if (Dist <= AttackRange)
    {
        Attack();
    }
}

void ATOHEnemy::Attack()
{
    float Now = GetWorld()->GetTimeSeconds();
    if (Now - LastAttackTime < AttackCooldown) return;
    LastAttackTime = Now;

    if (!ProjectileClass || !TargetPlayer) return;

    FVector Muzzle = GetActorLocation() + FVector(0, 0, 80);
    FVector Dir = (TargetPlayer->GetActorLocation() + FVector(0, 0, 60) - Muzzle).GetSafeNormal();
    FActorSpawnParameters Params;
    Params.Owner = this;
    ATOHProjectile* Proj = GetWorld()->SpawnActor<ATOHProjectile>(ProjectileClass, Muzzle, Dir.Rotation(), Params);
    if (Proj)
    {
        Proj->bIsEnemyProjectile = true;
        Proj->Damage = Damage;
    }
}

void ATOHEnemy::TakeDamage(float Amount)
{
    if (!IsAlive()) return;
    Health = FMath::Max(0.0f, Health - Amount);
    if (Health <= 0.0f)
    {
        Destroy();
    }
}
