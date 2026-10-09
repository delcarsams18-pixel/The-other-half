#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TOHHUD.generated.h"

UCLASS()
class TOH_API ATOHHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    void DrawHealthBar();
    void DrawObjective();
    void DrawEndScreen();
};
