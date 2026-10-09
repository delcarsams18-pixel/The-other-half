#include "TOHCarrieHologram.h"
#include "TOHArtLoader.h"
#include "TOHGLBLoader.h"
#include "CarrieMeshData.h"
#include "Materials/Material.h"
#include "ProceduralMeshComponent.h"
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
    
    CarrieModel = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CarrieModel"));
    CarrieModel->SetupAttachment(RootComponent);
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

    // Build Carrie 3D model from embedded mesh data
    if (CarrieModel)
    {
        TArray<FVector> Vertices;
        TArray<int32> Triangles;
        TArray<FVector> Normals;
        TArray<FVector2D> UVs;
        TArray<FLinearColor> Colors;
        TArray<FProcMeshTangent> Tangents;
        
        int32 NumVerts = sizeof(Carrie_Vertices) / sizeof(float) / 3;
        for (int32 i = 0; i < NumVerts; i++)
        {
            float GX = Carrie_Vertices[i*3];
            float GY = Carrie_Vertices[i*3+1];
            float GZ = Carrie_Vertices[i*3+2];
            Vertices.Add(FVector(GX * 100.0f, -GZ * 100.0f, GY * 100.0f));
            Normals.Add(FVector(0, 0, 1));
            UVs.Add(FVector2D(0, 0));
            Colors.Add(FLinearColor(0.8f, 0.1f, 0.1f));
            Tangents.Add(FProcMeshTangent(1, 0, 0));
        }
        int32 NumIdx = sizeof(Carrie_Indices) / sizeof(uint32);
        for (int32 i = 0; i < NumIdx; i++)
        {
            Triangles.Add((int32)Carrie_Indices[i]);
        }
        CarrieModel->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
        CarrieModel->SetRelativeLocation(FVector(0, 0, -50));
        if (HologramPlane)
        {
            HologramPlane->SetVisibility(false);
        }
    }

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
