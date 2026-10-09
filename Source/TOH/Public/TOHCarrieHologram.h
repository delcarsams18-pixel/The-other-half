#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TOHCarrieHologram.generated.h"

UCLASS()
class TOH_API ATOHCarrieHologram : public AActor
{
    GENERATED_BODY()

public:
    ATOHCarrieHologram();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "TOH")
    class UStaticMeshComponent* HologramPlane;

    UPROPERTY(VisibleAnywhere, Category = "TOH")
    class UPointLightComponent* GlowLight;
};
