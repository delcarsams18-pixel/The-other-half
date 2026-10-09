#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"

class TOH_API FTOHAndroid
{
public:
    static void InitializeAndroid();
    static bool IsAndroidDevice();
    static void SetupTouchInput();
};
