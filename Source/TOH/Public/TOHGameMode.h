#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TOHGameMode.generated.h"

UCLASS()
class TOH_API ATOHGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATOHGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintReadOnly, Category = "TOH")
    int32 EnemiesRemaining = 0;

    UPROPERTY(BlueprintReadOnly, Category = "TOH")
    bool bGameWon = false;

    UPROPERTY(BlueprintReadOnly, Category = "TOH")
    bool bGameLost = false;

    UFUNCTION(BlueprintCallable, Category = "TOH")
    void OnEnemyKilled();

    UFUNCTION(BlueprintCallable, Category = "TOH")
    void OnPlayerDied();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TOH")
    TSubclassOf<class ATOHEnemy> EnemyClass;

    UPROPERTY(EditDefaultsOnly, Category = "TOH")
    TSubclassOf<class ATOHProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "TOH")
    int32 NumEnemies = 8;

    void BuildDistrict();
    void SpawnEnemies();
    void SpawnCarrieMarker();
    void CheckWinLose();

    UPROPERTY()
    class AStaticMeshActor* CarrieMarker = nullptr;

    UPROPERTY()
    class ATOHCharacter* PlayerRef = nullptr;
};
