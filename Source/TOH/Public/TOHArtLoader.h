#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TOHArtLoader.generated.h"

UCLASS()
class TOH_API UTOHArtLoader : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "TOH|Art")
    static UTexture2D* LoadPNGFromFile(const FString& FileName);

    UFUNCTION(BlueprintCallable, Category = "TOH|Art")
    static FString GetArtPath(const FString& FileName);
};
