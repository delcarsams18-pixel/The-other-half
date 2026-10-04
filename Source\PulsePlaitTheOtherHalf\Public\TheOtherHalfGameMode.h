#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DistrictData.h"
#include "TheOtherHalfGameMode.generated.h"
UCLASS() class PULSEPLAITTOTHERHALF_API ATheOtherHalfGameMode : public AGameModeBase { GENERATED_BODY() public: ATheOtherHalfGameMode(); UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Lore") FString GameTitle="PULSE PLAIT: THE OTHER HALF - BENWAY MISSOURI YEAR 2526"; UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Lore") FString Lore="500 years from now, from rubbles after Trump failed policies corrupted, destabilized, bankrupted every country, plummeted into chaos"; UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Data") TArray<FHeroData> AllHeroes; UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Data") TArray<FDistrictData> AllDistricts; UFUNCTION(BlueprintCallable) void LoadAllData(); UFUNCTION(BlueprintCallable) void StartOperationRecover(); };