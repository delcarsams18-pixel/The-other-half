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
    if (!World) return;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane"));
    if (!CubeMesh.Succeeded()) return;

    auto MakeBox = [&](FVector Loc, FVector Scale, FLinearColor Color, float Emissive = 0.0f) -> AStaticMeshActor*
    {
        AStaticMeshActor* Box = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
        UStaticMeshComponent* Comp = Box->GetStaticMeshComponent();
        Comp->SetStaticMesh(CubeMesh.Object);
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

    if (PlaneMesh.Succeeded())
    {
        AStaticMeshActor* Ground = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
        UStaticMeshComponent* GComp = Ground->GetStaticMeshComponent();
        GComp->SetStaticMesh(PlaneMesh.Object);
        GComp->SetWorldScale3D(FVector(60.0f, 60.0f, 1.0f));
        UMaterialInstanceDynamic* GMat = GComp->CreateDynamicMaterialInstance(0);
        if (GMat)
        {
            GMat->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(0.02f, 0.02f, 0.03f));
        }
    }

    FMath::RandInit(1337);
    for (int32 i = 0; i < 24; i++)
    {
        float Angle = (i / 24.0f) * 2.0f * PI;
        float Radius = 1200.0f + FMath::RandRange(0.0f, 800.0f);
        float H = FMath::RandRange(400.0f, 1400.0f);
        float W = FMath::RandRange(200.0f, 400.0f);
        FVector Loc(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, H * 0.5f);
        FLinearColor BColor(0.03f, 0.04f, 0.06f);
        AStaticMeshActor* B = MakeBox(Loc, FVector(W / 100.0f, W / 100.0f, H / 100.0f), BColor);

        if (i % 3 == 0)
        {
            FLinearColor Neon = (i % 2 == 0) ? FLinearColor(0.1f, 0.5f, 1.0f) : FLinearColor(1.0f, 0.4f, 0.1f);
            MakeBox(Loc + FVector(0, 0, H * 0.5f + 10), FVector(W / 100.0f * 1.02f, W / 100.0f * 1.02f, 0.15f), Neon, 4.0f);
        }
    }

    ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector::ZeroVector, FRotator(-50.0f, -30.0f, 0.0f));
    Sun->GetLightComponent()->SetIntensity(0.4f);
    Sun->GetLightComponent()->SetLightColor(FLinearColor(0.4f, 0.5f, 0.8f));

    for (int32 i = 0; i < 8; i++)
    {
        float Angle = (i / 8.0f) * 2.0f * PI;
        FVector Loc(FMath::Cos(Angle) * 700.0f, FMath::Sin(Angle) * 700.0f, 300.0f);
        FLinearColor LC = (i % 2 == 0) ? FLinearColor(0.2f, 0.6f, 1.0f) : FLinearColor(1.0f, 0.5f, 0.15f);
        MakeBox(Loc, FVector(0.3f, 0.3f, 6.0f), FLinearColor(0.05f, 0.05f, 0.05f));
        MakeBox(Loc + FVector(0, 0, 320), FVector(1.5f, 1.5f, 0.5f), LC, 6.0f);
    }
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

    CarrieMarker = World->SpawnActor<ATOHCarrieHologram>(ATOHCarrieHologram::StaticClass(), FVector(0, 0, 150), FRotator::ZeroRotator);
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
