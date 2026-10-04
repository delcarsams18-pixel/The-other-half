#include "Hero.h"
ATOHGameMode::ATOHGameMode(){ DefaultPawnClass = nullptr; }
void ATOHGameMode::Load()
{
    AllHeroes.Empty();
    FTOHHero D; D.Id = TEXT("pulse"); D.Name = TEXT("Pulse Plait"); D.Role = TEXT("Benway"); D.Tech = TEXT("Stir"); D.A1 = TEXT("Stir"); D.A2 = TEXT("Hold"); D.A3 = TEXT("Strike"); D.Note = TEXT("Missouri");
    FTOHUnder u1; u1.Level = TEXT("T1"); u1.Name = TEXT("Under_T1"); u1.HP = 100; u1.DMG = 20; u1.Desc = TEXT("Basic");
    FTOHUnder u2; u2.Level = TEXT("T2"); u2.Name = TEXT("Under_T2"); u2.HP = 150; u2.DMG = 30; u2.Desc = TEXT("Mid");
    FTOHUnder u3; u3.Level = TEXT("T3"); u3.Name = TEXT("Under_T3"); u3.HP = 200; u3.DMG = 50; u3.Desc = TEXT("Elite");
    D.Unders.Add(u1); D.Unders.Add(u2); D.Unders.Add(u3);
    AllHeroes.Add(D);
}