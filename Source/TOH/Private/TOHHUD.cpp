#include "TOHHUD.h"
#include "TOHCharacter.h"
#include "TOHEnemy.h"
#include "TOHGameMode.h"
#include "TOHArtLoader.h"
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
    DrawCharacterArt();
    DrawEnemyMarkers();
    DrawEndScreen();
}

void ATOHHUD::DrawCharacterArt()
{
    if (LonzoPortrait && Canvas)
    {
        float Size = 160.0f;
        float X = Canvas->ClipX - Size - 20.0f;
        float Y = Canvas->ClipY - Size - 20.0f;
        DrawTexture(LonzoPortrait, X, Y, Size, Size, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White);
        DrawText(TEXT("LONZO"), FLinearColor(0.4f, 0.8f, 1.0f), X, Y - 22, nullptr, 1.1f, false);
    }
}

void ATOHHUD::DrawEnemyMarkers()
{
    if (VillainPortraits.Num() == 0 || !Canvas) return;

    APlayerController* PC = GetOwningPlayerController();
    if (!PC) return;

    TArray<AActor*> Enemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATOHEnemy::StaticClass(), Enemies);

    int32 Idx = 0;
    for (AActor* E : Enemies)
    {
        if (!E) continue;
        FVector WorldPos = E->GetActorLocation() + FVector(0, 0, 150);
        FVector2D ScreenPos;
        if (PC->ProjectWorldLocationToScreen(WorldPos, ScreenPos, true))
        {
            if (ScreenPos.X > 0 && ScreenPos.X < Canvas->ClipX && ScreenPos.Y > 0 && ScreenPos.Y < Canvas->ClipY)
            {
                UTexture2D* Portrait = VillainPortraits[Idx % VillainPortraits.Num()];
                float Size = 80.0f;
                DrawTexture(Portrait, ScreenPos.X - Size * 0.5f, ScreenPos.Y - Size * 0.5f,
                    Size, Size, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White);
            }
        }
        Idx++;
    }
}

ATOHHUD::ATOHHUD()
{
    LonzoPortrait = UTOHArtLoader::LoadPNGFromFile(TEXT("TOH_Lonzo_Full.png"));
    CarriePortrait = UTOHArtLoader::LoadPNGFromFile(TEXT("TOH_Carrie_Full.png"));

    TArray<FString> VillainFiles = {
        TEXT("TOH_Villain_Warden.png"),
        TEXT("TOH_Villain_NeonQueen.png"),
        TEXT("TOH_Villain_RustFather.png"),
        TEXT("TOH_Villain_Chemist.png"),
        TEXT("TOH_Villain_HoundMaster.png"),
        TEXT("TOH_Villain_BladeMother.png"),
        TEXT("TOH_Villain_SignalBreaker.png"),
        TEXT("TOH_Villain_Overlord.png"),
        TEXT("TOH_Underling_Brute.png"),
        TEXT("TOH_Underling_Striker.png"),
        TEXT("TOH_Underling_Vex.png"),
        TEXT("TOH_Underling_Jinx.png")
    };
    for (const FString& VF : VillainFiles)
    {
        UTexture2D* Tex = UTOHArtLoader::LoadPNGFromFile(VF);
        if (Tex)
        {
            VillainPortraits.Add(Tex);
        }
    }
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

    if (LonzoPortrait)
    {
        DrawTexture(LonzoPortrait, 30.0f, 80.0f, 90.0f, 140.0f,
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
