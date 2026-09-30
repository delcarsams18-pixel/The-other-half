#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheOtherHalfGameMode.generated.h"

UENUM(BlueprintType)
enum class EStoryAct : uint8 {
  BaseUpgradeOnly, PlainUSBTheft, RedAlertTaken, DemandedRobotics, NearDeathSyncFinal
};

UCLASS()
class THEOTHERHALF_API ATheOtherHalfGameMode : public AGameModeBase
{
  GENERATED_BODY()
public:
  UPROPERTY(EditAnywhere) EStoryAct CurrentAct = EStoryAct::BaseUpgradeOnly;
  UPROPERTY() float WorldSizeKM = 300.f; // Fenway City every speck grass houses enterable
  UPROPERTY() TArray<FString> Heroes = {
    "LONZO_Black_Glasses_BlueGatlingArm",
    "CARRIE_RedPonytail_BluePulse_Fly",
    "DARREL_Kennel_P7Hat_Fur_GoldMic",
    "DEACON_Blade_DualBlue",
    "FATHER_IN_LAW_White_Bald_Blue_Bunny_ThumbsUp",
    "BZ_Dreads_WhiteBucket_GreenBottles",
    "BIG_NATE_Skinny_PinkShorts_PinkShoes_EasterEgg"
  };
};
