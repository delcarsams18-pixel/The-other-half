#include "TOHGLBLoader.h"
#include "TOHArtLoader.h"
#include "ProceduralMeshComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"

FString UTOHGLBLoader::GetModelPath(const FString& FileName)
{
    FString BaseDir = FPaths::ProjectContentDir() + TEXT("Models/");
    FString FullPath = BaseDir + FileName;
    if (!FPaths::FileExists(FullPath))
    {
        FullPath = FPaths::ProjectDir() + TEXT("Content/Models/") + FileName;
    }
    return FullPath;
}

UProceduralMeshComponent* UTOHGLBLoader::LoadGLBAsMesh(UObject* Outer, const FString& FileName)
{
    FString FilePath = GetModelPath(FileName);

    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("GLBLoader: Cannot load %s"), *FilePath);
        return nullptr;
    }
    if (FileData.Num() < 12)
    {
        return nullptr;
    }

    const uint8* Ptr = FileData.GetData();
    uint32 Magic = *(uint32*)(Ptr);
    uint32 Version = *(uint32*)(Ptr + 4);
    if (Magic != 0x46546C67 || Version != 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("GLBLoader: Not a valid GLB"));
        return nullptr;
    }

    uint32 JsonLen = *(uint32*)(Ptr + 12);
    const uint8* JsonPtr = Ptr + 20;
    FString JsonStr;
    FFileHelper::BufferToString(JsonStr, JsonPtr, JsonLen);

    TSharedPtr<FJsonObject> JsonObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonStr);
    if (!FJsonSerializer::Deserialize(Reader, JsonObj) || !JsonObj.IsValid())
    {
        return nullptr;
    }

    uint32 BinOffset = 20 + JsonLen;
    if (BinOffset % 4) BinOffset += 4 - (BinOffset % 4);
    BinOffset += 8;
    const uint8* BinPtr = Ptr + BinOffset;

    auto Meshes = JsonObj->GetArrayField(TEXT("meshes"));
    if (Meshes.Num() == 0) return nullptr;
    auto Primitives = Meshes[0]->AsObject()->GetArrayField(TEXT("primitives"));
    if (Primitives.Num() == 0) return nullptr;
    auto Prim = Primitives[0]->AsObject();

    auto Accessors = JsonObj->GetArrayField(TEXT("accessors"));
    auto BufferViews = JsonObj->GetArrayField(TEXT("bufferViews"));

    auto Attrs = Prim->GetObjectField(TEXT("attributes"));
    int32 PosAccIdx = Attrs->GetIntegerField(TEXT("POSITION"));
    int32 UvAccIdx = Attrs->HasField(TEXT("TEXCOORD_0")) ? Attrs->GetIntegerField(TEXT("TEXCOORD_0")) : -1;
    int32 IdxAccIdx = Prim->GetIntegerField(TEXT("indices"));

    auto GetAccData = [&](int32 AccIdx, const uint8*& OutData, int32& OutCount, int32& OutStride)
    {
        auto Acc = Accessors[AccIdx]->AsObject();
        OutCount = Acc->GetIntegerField(TEXT("count"));
        int32 BvIdx = Acc->GetIntegerField(TEXT("bufferView"));
        auto Bv = BufferViews[BvIdx]->AsObject();
        int32 BvOffset = Bv->HasField(TEXT("byteOffset")) ? Bv->GetIntegerField(TEXT("byteOffset")) : 0;
        int32 AccOffset = Acc->HasField(TEXT("byteOffset")) ? Acc->GetIntegerField(TEXT("byteOffset")) : 0;
        OutData = BinPtr + BvOffset + AccOffset;
        FString Type = Acc->GetStringField(TEXT("type"));
        OutStride = (Type == TEXT("VEC3")) ? 12 : (Type == TEXT("VEC2")) ? 8 : 4;
    };

    const uint8* PosData; int32 PosCount, PosStride;
    GetAccData(PosAccIdx, PosData, PosCount, PosStride);

    const uint8* UvData = nullptr; int32 UvCount = 0, UvStride = 0;
    if (UvAccIdx >= 0) GetAccData(UvAccIdx, UvData, UvCount, UvStride);

    const uint8* IdxData; int32 IdxCount, IdxStride;
    GetAccData(IdxAccIdx, IdxData, IdxCount, IdxStride);
    auto IdxAcc = Accessors[IdxAccIdx]->AsObject();
    int32 IdxCompType = IdxAcc->GetIntegerField(TEXT("componentType"));

    TArray<FVector> Vertices;
    TArray<FVector2D> UVs;
    TArray<int32> Triangles;
    Vertices.Reserve(PosCount);
    UVs.Reserve(PosCount);

    for (int32 i = 0; i < PosCount; i++)
    {
        const float* P = (const float*)(PosData + i * PosStride);
        Vertices.Add(FVector(P[0], P[1], P[2]));
        if (UvData)
        {
            const float* Uv = (const float*)(UvData + i * UvStride);
            UVs.Add(FVector2D(Uv[0], Uv[1]));
        }
        else
        {
            UVs.Add(FVector2D::ZeroVector);
        }
    }

    Triangles.Reserve(IdxCount);
    for (int32 i = 0; i < IdxCount; i++)
    {
        int32 Idx = 0;
        if (IdxCompType == 5123)
        {
            Idx = *(uint16*)(IdxData + i * 2);
        }
        else if (IdxCompType == 5125)
        {
            Idx = *(uint32*)(IdxData + i * 4);
        }
        else if (IdxCompType == 5121)
        {
            Idx = *(uint8*)(IdxData + i);
        }
        Triangles.Add(Idx);
    }

    TArray<FVector> Normals;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    UProceduralMeshComponent* ProcMesh = NewObject<UProceduralMeshComponent>(Outer);
    if (!ProcMesh) return nullptr;

    ProcMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);

    UE_LOG(LogTemp, Log, TEXT("GLBLoader: Loaded %s - %d verts, %d tris"), *FileName, PosCount, IdxCount / 3);
    return ProcMesh;
}
