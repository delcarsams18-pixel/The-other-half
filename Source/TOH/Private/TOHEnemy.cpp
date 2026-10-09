#include "TOHEnemy.h"
#include "ProceduralMeshComponent.h"
#include "BrickMeshData.h"
#include "DeadeyeMeshData.h"
#include "ShivMeshData.h"
#include "HexMeshData.h"
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

    GunModel = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GunModel"));
    GunModel->SetupAttachment(GetMesh());

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

    // Build hired gun 3D model from embedded data
    if (GunModel)
    {
        // Random gun type if not set
        if (GunType < 0 || GunType > 3)
        {
            GunType = FMath::RandRange(0, 3);
        }
        
        const float* Verts = nullptr;
        const uint32* Idxs = nullptr;
        int32 NumVerts = 0;
        int32 NumIdx = 0;
        
        switch (GunType)
        {
        case 0:
            Verts = Brick_Vertices; Idxs = Brick_Indices;
            NumVerts = sizeof(Brick_Vertices)/sizeof(float)/3;
            NumIdx = sizeof(Brick_Indices)/sizeof(uint32);
            break;
        case 1:
            Verts = Deadeye_Vertices; Idxs = Deadeye_Indices;
            NumVerts = sizeof(Deadeye_Vertices)/sizeof(float)/3;
            NumIdx = sizeof(Deadeye_Indices)/sizeof(uint32);
            break;
        case 2:
            Verts = Shiv_Vertices; Idxs = Shiv_Indices;
            NumVerts = sizeof(Shiv_Vertices)/sizeof(float)/3;
            NumIdx = sizeof(Shiv_Indices)/sizeof(uint32);
            break;
        case 3:
            Verts = Hex_Vertices; Idxs = Hex_Indices;
            NumVerts = sizeof(Hex_Vertices)/sizeof(float)/3;
            NumIdx = sizeof(Hex_Indices)/sizeof(uint32);
            break;
        }
        
        if (Verts && Idxs)
        {
            TArray<FVector> Vertices;
            TArray<int32> Triangles;
            TArray<FVector> Normals;
            TArray<FVector2D> UVs;
            TArray<FLinearColor> Colors;
            TArray<FProcMeshTangent> Tangents;
            
            FLinearColor GunColor = FLinearColor::White;
            switch (GunType)
            {
            case 0: GunColor = FLinearColor(0.5f, 0.2f, 0.1f); break; // Brick: brown/red
            case 1: GunColor = FLinearColor(0.1f, 0.2f, 0.5f); break; // Deadeye: blue
            case 2: GunColor = FLinearColor(0.4f, 0.1f, 0.5f); break; // Shiv: purple
            case 3: GunColor = FLinearColor(0.1f, 0.5f, 0.2f); break; // Hex: green
            }
            for (int32 i = 0; i < NumVerts; i++)
            {
                float GX = Verts[i*3];
                float GY = Verts[i*3+1];
                float GZ = Verts[i*3+2];
                Vertices.Add(FVector(GX * 100.0f, -GZ * 100.0f, GY * 100.0f));
                Normals.Add(FVector(0, 0, 1));
                UVs.Add(FVector2D(0, 0));
                Colors.Add(GunColor);
                Tangents.Add(FProcMeshTangent(1, 0, 0));
            }
            for (int32 i = 0; i < NumIdx; i++)
            {
                Triangles.Add((int32)Idxs[i]);
            }
            GunModel->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
            GunModel->SetRelativeLocation(FVector(0, 0, -50));
            if (BodyMesh)
            {
                BodyMesh->SetVisibility(false);
            }
        }
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

void ATOHEnemy::ApplyDamage(float Amount)
{
    if (!IsAlive()) return;
    Health = FMath::Max(0.0f, Health - Amount);
    if (Health <= 0.0f)
    {
        Destroy();
    }
}
