#include "TOHCarrieHologram.h"
#include "TOHArtLoader.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

ATOHCarrieHologram::ATOHCarrieHologram()
{
    PrimaryActorTick.bCanEverTick = true;

    HologramPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HologramPlane"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane"));
    if (PlaneMesh.Succeeded())
    {
        HologramPlane->SetStaticMesh(PlaneMesh.Object);
        HologramPlane->SetRelativeScale3D(FVector(3.0f, 2.0f, 4.0f));
        HologramPlane->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
    }
    RootComponent = HologramPlane;
    HologramPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    GlowLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("GlowLight"));
    GlowLight->SetupAttachment(RootComponent);
    GlowLight->SetLightColor(FLinearColor(0.6f, 0.2f, 1.0f));
    GlowLight->SetIntensity(2000.0f);
    GlowLight->SetAttenuationRadius(800.0f);

    Tags.Add(TEXT("Carrie"));

    CarrieTexture = UTOHArtLoader::LoadPNGFromFile(TEXT("TOH_Carrie_Full.png"));
}

void ATOHCarrieHologram::BeginPlay()
{
    Super::BeginPlay();

    UMaterialInstanceDynamic* Mat = HologramPlane->CreateDynamicMaterialInstance(0);
    if (Mat)
    {
        if (CarrieTexture)
        {
            Mat->SetTextureParameterValue(TEXT("EmissiveColor"), CarrieTexture);
        }
        Mat->SetVectorParameterValue(TEXT("EmissiveColor"), FLinearColor(0.6f, 0.2f, 1.0f));
        Mat->SetScalarParameterValue(TEXT("EmissiveIntensity"), 2.0f);
    }
}

void ATOHCarrieHologram::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC && PC->PlayerCameraManager)
    {
        FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
        FVector ToCam = (CamLoc - GetActorLocation()).GetSafeNormal();
        FRotator FaceRot = FRotationMatrix::MakeFromX(ToCam).Rotator();
        SetActorRotation(FRotator(0.0f, FaceRot.Yaw, 0.0f));
    }
}
