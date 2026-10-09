#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TOHGLBLoader.generated.h"

UCLASS()
class TOH_API UTOHGLBLoader : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "TOH|3D")
    static class UProceduralMeshComponent* LoadGLBAsMesh(
        UObject* Outer,
        const FString& FileName);

    UFUNCTION(BlueprintCallable, Category = "TOH|3D")
    static FString GetModelPath(const FString& FileName);
};
