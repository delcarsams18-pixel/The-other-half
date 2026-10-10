#include "TOHHeroNPC.h"
#include "ProceduralMeshComponent.h"
#include "PMacMeshData.h"
#include "BZMeshData.h"
#include "AdamMeshData.h"
#include "DarrelMeshData.h"
#include "BigNateMeshData.h"
#include "DarrelPistolMeshData.h"
#include "AdamBunnyGunMeshData.h"
#include "BZBackpackMeshData.h"
#include "LGunMeshData.h"

ATOHHeroNPC::ATOHHeroNPC()
{
    PrimaryActorTick.bCanEverTick = true;
    HeroModel = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("HeroModel"));
    RootComponent = HeroModel;
    WeaponModel = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("WeaponModel"));
    WeaponModel->SetupAttachment(RootComponent);
}

void ATOHHeroNPC::BeginPlay()
{
    Super::BeginPlay();
    SpawnLoc = GetActorLocation();

    if (!HeroModel) return;

    const float* Verts = nullptr;
    const uint32* Idxs = nullptr;
    int32 NumVerts = 0;
    int32 NumIdx = 0;

    switch (HeroType)
    {
    case 0: // PMac
        Verts = PMac_Vertices; Idxs = PMac_Indices;
        NumVerts = sizeof(PMac_Vertices)/sizeof(float)/3;
        NumIdx = sizeof(PMac_Indices)/sizeof(uint32);
        HeroColor = FLinearColor(0.2f, 0.4f, 1.0f); // Blue
        break;
    case 1: // BZ
        Verts = BZ_Vertices; Idxs = BZ_Indices;
        NumVerts = sizeof(BZ_Vertices)/sizeof(float)/3;
        NumIdx = sizeof(BZ_Indices)/sizeof(uint32);
        HeroColor = FLinearColor(0.2f, 1.0f, 0.3f); // Green (herbs)
        break;
    case 2: // Adam
        Verts = Adam_Vertices; Idxs = Adam_Indices;
        NumVerts = sizeof(Adam_Vertices)/sizeof(float)/3;
        NumIdx = sizeof(Adam_Indices)/sizeof(uint32);
        HeroColor = FLinearColor(1.0f, 0.8f, 0.2f); // Gold
        break;
    case 3: // Darrel
        Verts = Darrel_Vertices; Idxs = Darrel_Indices;
        NumVerts = sizeof(Darrel_Vertices)/sizeof(float)/3;
        NumIdx = sizeof(Darrel_Indices)/sizeof(uint32);
        HeroColor = FLinearColor(0.6f, 0.3f, 0.1f); // Brown (dogs)
        break;
    case 4: // BigNate
        Verts = BigNate_Vertices; Idxs = BigNate_Indices;
        NumVerts = sizeof(BigNate_Vertices)/sizeof(float)/3;
        NumIdx = sizeof(BigNate_Indices)/sizeof(uint32);
        HeroColor = FLinearColor(1.0f, 0.5f, 0.1f); // Orange
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

        for (int32 i = 0; i < NumVerts; i++)
        {
            float GX = Verts[i*3];
            float GY = Verts[i*3+1];
            float GZ = Verts[i*3+2];
            Vertices.Add(FVector(GX * 100.0f, -GZ * 100.0f, GY * 100.0f));
            Normals.Add(FVector(0, 0, 1));
            UVs.Add(FVector2D(0, 0));
            Colors.Add(HeroColor);
            Tangents.Add(FProcMeshTangent(1, 0, 0));
        }
        for (int32 i = 0; i < NumIdx; i++)
        {
            Triangles.Add((int32)Idxs[i]);
        }
        HeroModel->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
        
        // Build weapon mesh
        const float* WVerts = nullptr;
        const uint32* WIdxs = nullptr;
        int32 WNumVerts = 0;
        int32 WNumIdx = 0;
        FVector WeaponOffset(50.0f, 30.0f, 100.0f); // Right hand area
        float WeaponScale = 1.0f;
        
        switch (HeroType)
        {
        case 0: // PMac -> LGun
            WVerts = LGun_Vertices; WIdxs = LGun_Indices;
            WNumVerts = sizeof(LGun_Vertices)/sizeof(float)/3;
            WNumIdx = sizeof(LGun_Indices)/sizeof(uint32);
            break;
        case 1: // BZ -> Backpack (on back)
            WVerts = BZBackpack_Vertices; WIdxs = BZBackpack_Indices;
            WNumVerts = sizeof(BZBackpack_Vertices)/sizeof(float)/3;
            WNumIdx = sizeof(BZBackpack_Indices)/sizeof(uint32);
            WeaponOffset = FVector(-40.0f, 0.0f, 140.0f); // On back
            break;
        case 2: // Adam -> BunnyGun
            WVerts = AdamBunnyGun_Vertices; WIdxs = AdamBunnyGun_Indices;
            WNumVerts = sizeof(AdamBunnyGun_Vertices)/sizeof(float)/3;
            WNumIdx = sizeof(AdamBunnyGun_Indices)/sizeof(uint32);
            break;
        case 3: // Darrel -> Pistol
            WVerts = DarrelPistol_Vertices; WIdxs = DarrelPistol_Indices;
            WNumVerts = sizeof(DarrelPistol_Vertices)/sizeof(float)/3;
            WNumIdx = sizeof(DarrelPistol_Indices)/sizeof(uint32);
            WeaponScale = 1.5f;
            break;
        case 4: // BigNate -> no weapon
            break;
        }
        
        if (WVerts && WIdxs && WeaponModel)
        {
            TArray<FVector> WVertices;
            TArray<int32> WTriangles;
            TArray<FVector> WNormals;
            TArray<FVector2D> WUVs;
            TArray<FLinearColor> WColors;
            TArray<FProcMeshTangent> WTangents;
            for (int32 i = 0; i < WNumVerts; i++)
            {
                float GX = WVerts[i*3] * WeaponScale;
                float GY = WVerts[i*3+1] * WeaponScale;
                float GZ = WVerts[i*3+2] * WeaponScale;
                WVertices.Add(FVector(GX * 100.0f, -GZ * 100.0f, GY * 100.0f) + WeaponOffset);
                WNormals.Add(FVector(0, 0, 1));
                WUVs.Add(FVector2D(0, 0));
                WColors.Add(FLinearColor(0.3f, 0.3f, 0.35f));
                WTangents.Add(FProcMeshTangent(1, 0, 0));
            }
            for (int32 i = 0; i < WNumIdx; i++) WTriangles.Add((int32)WIdxs[i]);
            WeaponModel->CreateMeshSection_LinearColor(0, WVertices, WTriangles, WNormals, WUVs, WColors, WTangents, true);
        }
        
        // Use solid color material (not vertex colors which can appear transparent)
        UMaterialInstanceDynamic* HeroMat = HeroModel->CreateDynamicMaterialInstance(0);
        if (HeroMat)
        {
            HeroMat->SetVectorParameterValue(TEXT("BaseColor"), HeroColor);
        }
    }
}

void ATOHHeroNPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    BobTime += DeltaTime;
    
    // Wander around spawn point
    if (!bHasTarget || FVector::Dist(GetActorLocation(), TargetLoc) < 100.0f)
    {
        // Pick new target near spawn
        float Angle = FMath::RandRange(0.0f, 2.0f * PI);
        float Dist = FMath::RandRange(200.0f, 800.0f);
        TargetLoc = SpawnLoc + FVector(FMath::Cos(Angle) * Dist, FMath::Sin(Angle) * Dist, 0);
        bHasTarget = true;
    }
    
    // Move toward target
    FVector MyLoc = GetActorLocation();
    FVector Dir = (TargetLoc - MyLoc).GetSafeNormal();
    Dir.Z = 0;
    FVector NewLoc = MyLoc + Dir * 150.0f * DeltaTime;
    NewLoc.Z = MyLoc.Z; // Keep height
    SetActorLocation(NewLoc);
    
    // Face movement direction
    if (Dir.SizeSquared() > 0.01f)
    {
        FRotator NewRot = Dir.Rotation();
        NewRot.Pitch = 0; NewRot.Roll = 0;
        SetActorRotation(NewRot);
    }
}
