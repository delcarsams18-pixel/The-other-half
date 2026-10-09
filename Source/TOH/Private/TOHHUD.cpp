#include "TOHHUD.h"
#include "TOHCharacter.h"
#include "TOHGameMode.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

void ATOHHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas) return;

    DrawHealthBar();
    DrawObjective();
    DrawEndScreen();
}

void ATOHHUD::DrawHealthBar()
{
    ATOHCharacter* Player = Cast<ATOHCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Player) return;

    float Pct = Player->Health / Player->MaxHealth;
    float W = 300.0f * Pct;
    FVector2D Pos(30.0f, Canvas->ClipY - 60.0f);

    DrawRect(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f), Pos.X - 2, Pos.Y - 2, 304, 24);
    FLinearColor BarColor = Pct > 0.5f ? FLinearColor(0.2f, 0.8f, 0.3f) : FLinearColor(0.9f, 0.2f, 0.1f);
    DrawRect(BarColor, Pos.X, Pos.Y, W, 20);

    FString HPText = FString::Printf(TEXT("HP: %d"), FMath::RoundToInt(Player->Health));
    DrawText(HPText, FLinearColor::White, Pos.X, Pos.Y - 22, nullptr, 1.2f, false);

    static ConstructorHelpers::FObjectFinder<UTexture2D> LonzoTex(TEXT("/Game/Textures/TOH_Lonzo"));
    if (LonzoTex.Succeeded())
    {
        DrawTexture(LonzoTex.Object, 30.0f, 80.0f, 90.0f, 140.0f,
            0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White);
    }
}

void ATOHHUD::DrawObjective()
{
    ATOHGameMode* GM = Cast<ATOHGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (!GM || GM->bGameWon || GM->bGameLost) return;

    FString Obj;
    if (GM->EnemiesRemaining > 0)
    {
        Obj = FString::Printf(TEXT("OPERATION RECOVER: Eliminate hostiles (%d left)"), GM->EnemiesRemaining);
    }
    else
    {
        Obj = TEXT("OPERATION RECOVER: Reach Carrie (purple beacon)");
    }
    DrawText(Obj, FLinearColor(0.4f, 0.8f, 1.0f), 30.0f, 30.0f, nullptr, 1.4f, false);
    DrawText(TEXT("THE OTHER HALF"), FLinearColor(1.0f, 0.6f, 0.1f), 30.0f, 55.0f, nullptr, 1.1f, false);
}

void ATOHHUD::DrawEndScreen()
{
    ATOHGameMode* GM = Cast<ATOHGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (!GM) return;

    if (GM->bGameWon)
    {
        FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.4f);
        DrawText(TEXT("CARRIE RECOVERED"), FLinearColor(0.6f, 0.3f, 1.0f), Center.X - 150, Center.Y, nullptr, 2.5f, false);
        DrawText(TEXT("Operation Recover: Phase 1 Complete"), FLinearColor::White, Center.X - 170, Center.Y + 50, nullptr, 1.3f, false);
    }
    else if (GM->bGameLost)
    {
        FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.4f);
        DrawText(TEXT("SIGNAL LOST"), FLinearColor(1.0f, 0.2f, 0.1f), Center.X - 110, Center.Y, nullptr, 2.5f, false);
        DrawText(TEXT("Lonzo is down. Retry Operation Recover."), FLinearColor::White, Center.X - 170, Center.Y + 50, nullptr, 1.3f, false);
    }
}
