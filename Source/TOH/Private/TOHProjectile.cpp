#include "TOHProjectile.h"
#include "TOHCharacter.h"
#include "TOHEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

ATOHProjectile::ATOHProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->InitSphereRadius(12.0f);
    Collision->SetCollisionProfileName(TEXT("Projectile"));
    RootComponent = Collision;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
    if (SphereMesh.Succeeded())
    {
        Mesh->SetStaticMesh(SphereMesh.Object);
        Mesh->SetRelativeScale3D(FVector(0.35f));
    }
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
    MoveComp->InitialSpeed = Speed;
    MoveComp->MaxSpeed = Speed;
    MoveComp->bRotationFollowsVelocity = true;
    MoveComp->ProjectileGravityScale = 0.0f;

    Collision->OnComponentHit.AddDynamic(this, &ATOHProjectile::OnHit);
}

void ATOHProjectile::BeginPlay()
{
    Super::BeginPlay();
    MoveComp->InitialSpeed = Speed;
    MoveComp->MaxSpeed = Speed;
    MoveComp->Velocity = GetActorForwardVector() * Speed;

    UMaterialInstanceDynamic* Mat = Mesh->CreateDynamicMaterialInstance(0);
    if (Mat)
    {
        if (bIsEnemyProjectile)
        {
            Mat->SetVectorParameterValue(TEXT("EmissiveColor"), FLinearColor(1.0f, 0.15f, 0.1f));
        }
        else
        {
            Mat->SetVectorParameterValue(TEXT("EmissiveColor"), FLinearColor(0.2f, 0.6f, 1.0f));
        }
        Mat->SetScalarParameterValue(TEXT("EmissiveIntensity"), 5.0f);
    }
}

void ATOHProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Age += DeltaTime;
    if (Age >= LifeTime)
    {
        Destroy();
    }
}

void ATOHProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == GetOwner()) return;

    if (bIsEnemyProjectile)
    {
        ATOHCharacter* Player = Cast<ATOHCharacter>(OtherActor);
        if (Player)
        {
            Player->ApplyDamage(Damage);
        }
    }
    else
    {
        ATOHEnemy* Enemy = Cast<ATOHEnemy>(OtherActor);
        if (Enemy)
        {
            Enemy->ApplyDamage(Damage);
        }
    }
    Destroy();
}
