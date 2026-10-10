#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TOHHeroNPC.generated.h"

UCLASS()
class TOH_API ATOHHeroNPC : public AActor
{
    GENERATED_BODY()

public:
    ATOHHeroNPC();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TOH")
    int32 HeroType = 0; // 0=PMac, 1=BZ, 2=Adam, 3=Darrel, 4=BigNate

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TOH")
    FLinearColor HeroColor = FLinearColor::White;

protected:
    UPROPERTY(VisibleAnywhere, Category = "TOH")
    class UProceduralMeshComponent* HeroModel;

    float BobTime = 0.0f;
    FVector SpawnLoc = FVector::ZeroVector;
    FVector TargetLoc = FVector::ZeroVector;
    bool bHasTarget = false;
};
