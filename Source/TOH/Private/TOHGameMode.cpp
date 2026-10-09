#include "TOHGameMode.h"
#include "TOHCharacter.h"
#include "TOHEnemy.h"
#include "TOHProjectile.h"
#include "TOHCarrieHologram.h"
#include "TOHHUD.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/DirectionalLightComponent.h"

ATOHGameMode::ATOHGameMode()
{
    DefaultPawnClass = ATOHCharacter::StaticClass();
    HUDClass = ATOHHUD::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();

    EnemyClass = ATOHEnemy::StaticClass();
    ProjectileClass = ATOHProjectile::StaticClass();

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshObj(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMeshObj.Succeeded())
    {
        CubeMeshAsset = CubeMeshObj.Object;
    }
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshObj(TEXT("/Engine/BasicShapes/Plane"));
    if (PlaneMeshObj.Succeeded())
    {
        PlaneMeshAsset = PlaneMeshObj.Object;
    }
}

void ATOHGameMode::BeginPlay()
{
    Super::BeginPlay();
    PlayerRef = Cast<ATOHCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    BuildDistrict();
    SpawnEnemies();
    SpawnCarrieMarker();
}

void ATOHGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckWinLose();
}

void ATOHGameMode::BuildDistrict()
{
    UWorld* World = GetWorld();
    if (!World || !CubeMeshAsset) return;

    auto MakeBox = [&](FVector Loc, FVector Scale, FLinearColor Color, float Emissive = 0.0f) -> AStaticMeshActor*
    {
        AStaticMeshActor* Box = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
        UStaticMeshComponent* Comp = Box->GetStaticMeshComponent();
        Comp->SetStaticMesh(CubeMeshAsset);
        Comp->SetWorldScale3D(Scale);
        Comp->SetMobility(EComponentMobility::Static);
        UMaterialInstanceDynamic* Mat = Comp->CreateDynamicMaterialInstance(0);
        if (Mat)
        {
            Mat->SetVectorParameterValue(TEXT("BaseColor"), Color);
            if (Emissive > 0.0f)
            {
                Mat->SetVectorParameterValue(TEXT("EmissiveColor"), Color);
                Mat->SetScalarParameterValue(TEXT("EmissiveIntensity"), Emissive);
            }
        }
        return Box;
    };

    // Ground - city streets (dark asphalt)
    if (PlaneMeshAsset)
    {
        AStaticMeshActor* Ground = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
        UStaticMeshComponent* GComp = Ground->GetStaticMeshComponent();
        GComp->SetStaticMesh(PlaneMeshAsset);
        GComp->SetWorldScale3D(FVector(110.0f, 110.0f, 1.0f));
        UMaterialInstanceDynamic* GMat = GComp->CreateDynamicMaterialInstance(0);
        if (GMat)
        {
            GMat->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(0.03f, 0.03f, 0.04f));
        }
    }

    // City grid: 7x7 blocks with streets between, district variation
    FMath::RandInit(1337);
    const float BlockSize = 1200.0f;
    const float StreetWidth = 400.0f;
    const float CellSize = BlockSize + StreetWidth;
    
    for (int32 gx = -3; gx <= 3; gx++)
    {
        for (int32 gy = -3; gy <= 3; gy++)
        {
            // Skip center block (plaza/spawn area)
            if (gx == 0 && gy == 0) continue;
            
            // District type based on position
            // North (gy>0): Corporate (tall, blue)
            // South (gy<0): Industrial (short, orange/rust)
            // East (gx>0): Neon/Entertainment (colorful)
            // West (gx<0): Residential (medium, warm)
            int32 District = 0;
            if (gy >= 2) District = 1;      // North corporate
            else if (gy <= -2) District = 2; // South industrial
            else if (gx >= 2) District = 3;  // East neon
            else if (gx <= -2) District = 4; // West residential
            
            float BX = gx * CellSize;
            float BY = gy * CellSize;
            
            // Each block has 1-4 buildings
            int32 NumBuildings = FMath::RandRange(1, 3);
            for (int32 b = 0; b < NumBuildings; b++)
            {
                float BW = FMath::RandRange(300.0f, 700.0f);
                float BD = FMath::RandRange(300.0f, 700.0f);
                float BH = FMath::RandRange(500.0f, 2000.0f);
                float OX = FMath::RandRange(-200.0f, 200.0f);
                float OY = FMath::RandRange(-200.0f, 200.0f);
                
                FVector Loc(BX + OX, BY + OY, BH * 0.5f);
                // Building colors: dark grays/blues with variation
                float V = FMath::RandRange(0.03f, 0.08f);
                FLinearColor BColor(V, V * 1.2f, V * 1.5f);
                MakeBox(Loc, FVector(BW / 100.0f, BD / 100.0f, BH / 100.0f), BColor);
                
                // Neon signs on some buildings
                if (FMath::RandRange(0, 2) == 0)
                {
                    FLinearColor Neon;
                    int32 NC = FMath::RandRange(0, 3);
                    if (NC == 0) Neon = FLinearColor(0.1f, 0.5f, 1.0f);      // Blue
                    else if (NC == 1) Neon = FLinearColor(1.0f, 0.2f, 0.5f); // Pink
                    else if (NC == 2) Neon = FLinearColor(0.2f, 1.0f, 0.5f); // Green
                    else Neon = FLinearColor(1.0f, 0.6f, 0.1f);             // Orange
                    float SignH = BH * FMath::RandRange(0.6f, 0.9f);
                    MakeBox(FVector(BX + OX, BY + OY + BD/2 + 5, SignH), FVector(BW/100.0f*0.8f, 0.1f, 0.4f), Neon, 5.0f);
                }
                
                // Windows (emissive strips)
                if (BH > 800.0f)
                {
                    int32 Floors = (int32)(BH / 150.0f);
                    for (int32 fl = 0; fl < Floors; fl += 2)
                    {
                        float WY = BH * 0.1f + fl * 150.0f;
                        if (WY < BH * 0.95f)
                        {
                            FLinearColor WinColor(1.0f, 0.8f, 0.4f);
                            MakeBox(FVector(BX + OX, BY + OY + BD/2 + 2, WY), FVector(BW/100.0f*0.9f, 0.05f, 0.2f), WinColor, 2.0f);
                        }
                    }
                }
            }
        }
    }
    
    // Street lights along main roads
    for (int32 i = -3; i <= 3; i++)
    {
        float Pos = i * CellSize;
        // X-axis street lights
        MakeBox(FVector(Pos, -StreetWidth/2, 300), FVector(0.3f, 0.3f, 6.0f), FLinearColor(0.05f, 0.05f, 0.05f));
        MakeBox(FVector(Pos, -StreetWidth/2, 620), FVector(1.2f, 1.2f, 0.4f), FLinearColor(0.3f, 0.6f, 1.0f), 6.0f);
        MakeBox(FVector(Pos, StreetWidth/2, 300), FVector(0.3f, 0.3f, 6.0f), FLinearColor(0.05f, 0.05f, 0.05f));
        MakeBox(FVector(Pos, StreetWidth/2, 620), FVector(1.2f, 1.2f, 0.4f), FLinearColor(0.3f, 0.6f, 1.0f), 6.0f);
        // Z-axis street lights
        MakeBox(FVector(-StreetWidth/2, Pos, 300), FVector(0.3f, 0.3f, 6.0f), FLinearColor(0.05f, 0.05f, 0.05f));
        MakeBox(FVector(-StreetWidth/2, Pos, 620), FVector(1.2f, 1.2f, 0.4f), FLinearColor(1.0f, 0.5f, 0.15f), 6.0f);
        MakeBox(FVector(StreetWidth/2, Pos, 300), FVector(0.3f, 0.3f, 6.0f), FLinearColor(0.05f, 0.05f, 0.05f));
        MakeBox(FVector(StreetWidth/2, Pos, 620), FVector(1.2f, 1.2f, 0.4f), FLinearColor(1.0f, 0.5f, 0.15f), 6.0f);
    }

    // Health pickups (green glowing cubes) scattered through the city
    for (int32 i = 0; i < 12; i++)
    {
        float PX = FMath::RandRange(-4800.0f, 4800.0f);
        float PY = FMath::RandRange(-4800.0f, 4800.0f);
        // Keep clear of center spawn
        if (FMath::Abs(PX) < 800.0f && FMath::Abs(PY) < 800.0f) continue;
        AStaticMeshActor* Pickup = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector(PX, PY, 80), FRotator::ZeroRotator);
        UStaticMeshComponent* PComp = Pickup->GetStaticMeshComponent();
        PComp->SetStaticMesh(CubeMeshAsset);
        PComp->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
        Pickup->Tags.Add(FName("HealthPickup"));
        UMaterialInstanceDynamic* PMat = PComp->CreateDynamicMaterialInstance(0);
        if (PMat)
        {
            FLinearColor Green(0.1f, 1.0f, 0.3f);
            PMat->SetVectorParameterValue(TEXT("BaseColor"), Green);
            PMat->SetVectorParameterValue(TEXT("EmissiveColor"), Green);
            PMat->SetScalarParameterValue(TEXT("EmissiveIntensity"), 4.0f);
        }
    }

    // Lighting
    ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector::ZeroVector, FRotator(-50.0f, -30.0f, 0.0f));
    Sun->GetLightComponent()->SetIntensity(0.35f);
    Sun->GetLightComponent()->SetLightColor(FLinearColor(0.4f, 0.5f, 0.8f));
}


void ATOHGameMode::SpawnEnemies()
{
    UWorld* World = GetWorld();
    if (!World || !EnemyClass) return;

    EnemiesRemaining = NumEnemies;
    for (int32 i = 0; i < NumEnemies; i++)
    {
        float Angle = FMath::RandRange(0.0f, 2.0f * PI);
        float Radius = FMath::RandRange(600.0f, 1800.0f);
        FVector Loc(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 100.0f);
        FActorSpawnParameters Params;
        ATOHEnemy* E = World->SpawnActor<ATOHEnemy>(EnemyClass, Loc, FRotator::ZeroRotator, Params);
    }
}

void ATOHGameMode::SpawnCarrieMarker()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // Carrie is held at the north end of the city - player must fight through to reach her
    CarrieMarker = World->SpawnActor<ATOHCarrieHologram>(ATOHCarrieHologram::StaticClass(), FVector(0, 3200, 100), FRotator::ZeroRotator);
    
    // Hero team NPCs in the central plaza (using enemy class as base, friendly)
    // They use the embedded hero mesh data
    struct FHeroSpawn { const TCHAR* Name; float X; float Y; int32 MeshIdx; };
    // MeshIdx: 0=PMac, 1=BZ, 2=Adam, 3=Darrel, 4=BigNate (handled in enemy code via GunType offset)
    // For now, spawn as visual-only actors
}

void ATOHGameMode::OnEnemyKilled()
{
    EnemiesRemaining = FMath::Max(0, EnemiesRemaining - 1);
}

void ATOHGameMode::OnPlayerDied()
{
    bGameLost = true;
}

void ATOHGameMode::CheckWinLose()
{
    if (bGameWon || bGameLost || !PlayerRef) return;

    if (!PlayerRef->IsAlive())
    {
        OnPlayerDied();
        return;
    }

    TArray<AActor*> Enemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATOHEnemy::StaticClass(), Enemies);
    EnemiesRemaining = Enemies.Num();

    if (EnemiesRemaining == 0 && CarrieMarker)
    {
        float Dist = FVector::Dist(PlayerRef->GetActorLocation(), CarrieMarker->GetActorLocation());
        if (Dist < 400.0f)
        {
            bGameWon = true;
        }
    }
}
