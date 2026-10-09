#include "TOHGameMode.h"
#include "TOHHeroNPC.h"
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
        GComp->SetWorldScale3D(FVector(140.0f, 140.0f, 1.0f));
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
    
    for (int32 gx = -4; gx <= 4; gx++)
    {
        for (int32 gy = -4; gy <= 4; gy++)
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
    for (int32 i = -4; i <= 4; i++)
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

    // DETAIL PASS: sidewalks, cars, billboards, rooftop elements, street props
    
    // Sidewalks along main streets (lighter gray strips)
    for (int32 i = -4; i <= 4; i++)
    {
        float Pos = i * CellSize;
        // X-direction sidewalks
        MakeBox(FVector(Pos, -StreetWidth/2 - 60, 5), FVector(12.0f, 1.2f, 0.1f), FLinearColor(0.08f, 0.08f, 0.09f));
        MakeBox(FVector(Pos, StreetWidth/2 + 60, 5), FVector(12.0f, 1.2f, 0.1f), FLinearColor(0.08f, 0.08f, 0.09f));
        // Z-direction sidewalks  
        MakeBox(FVector(-StreetWidth/2 - 60, Pos, 5), FVector(1.2f, 12.0f, 0.1f), FLinearColor(0.08f, 0.08f, 0.09f));
        MakeBox(FVector(StreetWidth/2 + 60, Pos, 5), FVector(1.2f, 12.0f, 0.1f), FLinearColor(0.08f, 0.08f, 0.09f));
    }
    
    // Parked cars (simple car shapes: body + cabin)
    FMath::RandInit(777);
    for (int32 i = 0; i < 24; i++)
    {
        float CX = FMath::RandRange(-6000.0f, 6000.0f);
        float CY = FMath::RandRange(-6000.0f, 6000.0f);
        // Snap to street edges
        int32 Street = FMath::RandRange(0, 1);
        if (Street == 0) CY = (FMath::RandRange(-4, 4) * CellSize) + StreetWidth/2 + 120;
        else CX = (FMath::RandRange(-4, 4) * CellSize) + StreetWidth/2 + 120;
        
        float CarYaw = (Street == 0) ? 0.0f : 90.0f;
        FLinearColor CarColor(FMath::RandRange(0.05f, 0.3f), FMath::RandRange(0.05f, 0.3f), FMath::RandRange(0.05f, 0.4f));
        
        // Car body
        AStaticMeshActor* CarBody = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector(CX, CY, 60), FRotator(0, CarYaw, 0));
        UStaticMeshComponent* CBComp = CarBody->GetStaticMeshComponent();
        CBComp->SetStaticMesh(CubeMeshAsset);
        CBComp->SetWorldScale3D(FVector(4.5f, 2.0f, 1.2f));
        UMaterialInstanceDynamic* CBMat = CBComp->CreateDynamicMaterialInstance(0);
        if (CBMat) CBMat->SetVectorParameterValue(TEXT("BaseColor"), CarColor);
        
        // Car cabin
        AStaticMeshActor* CarTop = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector(CX, CY, 150), FRotator(0, CarYaw, 0));
        UStaticMeshComponent* CTComp = CarTop->GetStaticMeshComponent();
        CTComp->SetStaticMesh(CubeMeshAsset);
        CTComp->SetWorldScale3D(FVector(2.5f, 1.8f, 0.8f));
        UMaterialInstanceDynamic* CTMat = CTComp->CreateDynamicMaterialInstance(0);
        if (CTMat) CTMat->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(0.05f, 0.08f, 0.12f));
    }
    
    // Billboards on buildings
    for (int32 i = 0; i < 10; i++)
    {
        float BX = FMath::RandRange(-5000.0f, 5000.0f);
        float BY = FMath::RandRange(-5000.0f, 5000.0f);
        float BH = FMath::RandRange(800.0f, 1500.0f);
        FLinearColor AdColor;
        int32 AC = FMath::RandRange(0, 3);
        if (AC == 0) AdColor = FLinearColor(1.0f, 0.2f, 0.8f);
        else if (AC == 1) AdColor = FLinearColor(0.2f, 0.8f, 1.0f);
        else if (AC == 2) AdColor = FLinearColor(1.0f, 0.8f, 0.2f);
        else AdColor = FLinearColor(0.5f, 1.0f, 0.3f);
        // Billboard pole
        MakeBox(FVector(BX, BY, BH * 0.5f), FVector(0.4f, 0.4f, BH / 100.0f), FLinearColor(0.05f, 0.05f, 0.05f));
        // Billboard screen
        MakeBox(FVector(BX, BY, BH + 50), FVector(6.0f, 0.3f, 3.0f), AdColor, 3.0f);
    }
    
    // Rooftop details: water towers and AC units on random buildings
    for (int32 i = 0; i < 15; i++)
    {
        float RX = FMath::RandRange(-5000.0f, 5000.0f);
        float RY = FMath::RandRange(-5000.0f, 5000.0f);
        float RH = FMath::RandRange(1000.0f, 1800.0f);
        // Water tower (cylinder-ish box on legs)
        MakeBox(FVector(RX, RY, RH + 100), FVector(1.5f, 1.5f, 2.0f), FLinearColor(0.15f, 0.1f, 0.08f));
        MakeBox(FVector(RX, RY, RH + 250), FVector(2.0f, 2.0f, 1.0f), FLinearColor(0.2f, 0.12f, 0.08f));
        // AC unit
        MakeBox(FVector(RX + 200, RY + 150, RH + 40), FVector(1.2f, 1.0f, 0.8f), FLinearColor(0.12f, 0.12f, 0.14f));
    }
    
    // Street props: benches and trash cans in plaza area
    for (int32 i = 0; i < 8; i++)
    {
        float Angle = (i / 8.0f) * 2.0f * PI;
        float PX = FMath::Cos(Angle) * 700.0f;
        float PY = FMath::Sin(Angle) * 700.0f;
        // Bench
        MakeBox(FVector(PX, PY, 50), FVector(2.0f, 0.6f, 0.5f), FLinearColor(0.12f, 0.08f, 0.05f));
        // Trash can
        MakeBox(FVector(PX + 150, PY + 100, 60), FVector(0.6f, 0.6f, 1.2f), FLinearColor(0.08f, 0.1f, 0.08f));
    }

    // Health pickups (green glowing cubes) scattered through the city
    for (int32 i = 0; i < 20; i++)
    {
        float PX = FMath::RandRange(-6400.0f, 6400.0f);
        float PY = FMath::RandRange(-6400.0f, 6400.0f);
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
    
    // Hero team NPCs in the central plaza
    for (int32 h = 0; h < 5; h++)
    {
        float Angle = (h / 5.0f) * 2.0f * PI;
        float HX = FMath::Cos(Angle) * 400.0f;
        float HY = FMath::Sin(Angle) * 400.0f;
        ATOHHeroNPC* Hero = World->SpawnActor<ATOHHeroNPC>(ATOHHeroNPC::StaticClass(), FVector(HX, HY, 100), FRotator(0, Angle * 180.0f / PI + 90.0f, 0));
        if (Hero)
        {
            Hero->HeroType = h;
        }
    }
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
