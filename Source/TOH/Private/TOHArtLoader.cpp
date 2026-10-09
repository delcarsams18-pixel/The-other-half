#include "TOHArtLoader.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/Texture2D.h"
#include "RenderingThread.h"

FString UTOHArtLoader::GetArtPath(const FString& FileName)
{
    FString BaseDir = FPaths::ProjectContentDir() + TEXT("Textures/");
    FString FullPath = BaseDir + FileName;

    if (!FPaths::FileExists(FullPath))
    {
        FullPath = FPaths::ProjectDir() + TEXT("Content/Textures/") + FileName;
    }

    return FullPath;
}

UTexture2D* UTOHArtLoader::LoadPNGFromFile(const FString& FileName)
{
    FString FilePath = GetArtPath(FileName);

    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("TOHArtLoader: Failed to load file %s"), *FilePath);
        return nullptr;
    }

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (!ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
    {
        UE_LOG(LogTemp, Warning, TEXT("TOHArtLoader: Failed to decode PNG %s"), *FilePath);
        return nullptr;
    }

    TArray<uint8> RawData;
    if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
    {
        UE_LOG(LogTemp, Warning, TEXT("TOHArtLoader: Failed to get raw data %s"), *FilePath);
        return nullptr;
    }

    int32 Width = ImageWrapper->GetWidth();
    int32 Height = ImageWrapper->GetHeight();

    UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
    if (!Texture)
    {
        return nullptr;
    }

    void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
    Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
    Texture->UpdateResource();

    UE_LOG(LogTemp, Log, TEXT("TOHArtLoader: Loaded %s (%dx%d)"), *FilePath, Width, Height);
    return Texture;
}
