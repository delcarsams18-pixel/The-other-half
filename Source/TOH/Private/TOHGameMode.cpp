#include "Hero.h"

ATOHGameMode::ATOHGameMode()
{
    DefaultPawnClass = nullptr;
}

void ATOHGameMode::Load()
{
    AllHeroes.Empty();
    Villains.Empty();
    Districts.Empty();

    auto AddUnder = [](FTOHUnder& Under, const FString& InLevel, const FString& InName, int32 InHP, int32 InDMG, const FString& InDesc)
    {
        Under.Level = InLevel;
        Under.Name = InName;
        Under.HP = InHP;
        Under.DMG = InDMG;
        Under.Desc = InDesc;
    };

    auto AddHero = [&](const FString& InId, const FString& InName, const FString& InRole, const FString& InTech,
        const FString& InA1, const FString& InA2, const FString& InA3, const FString& InNote,
        const FString& InDistrict, const FString& InFaction)
    {
        FTOHHero Hero;
        Hero.Id = InId;
        Hero.Name = InName;
        Hero.Role = InRole;
        Hero.Tech = InTech;
        Hero.A1 = InA1;
        Hero.A2 = InA2;
        Hero.A3 = InA3;
        Hero.Note = InNote;
        Hero.District = InDistrict;
        Hero.Faction = InFaction;

        FTOHUnder U1, U2, U3;
        AddUnder(U1, TEXT("T1"), TEXT("Under_T1"), 100, 20, TEXT("Basic"));
        AddUnder(U2, TEXT("T2"), TEXT("Under_T2"), 150, 35, TEXT("Mid"));
        AddUnder(U3, TEXT("T3"), TEXT("Under_T3"), 200, 50, TEXT("Elite"));

        Hero.Unders.Add(U1);
        Hero.Unders.Add(U2);
        Hero.Unders.Add(U3);

        AllHeroes.Add(Hero);
    };

    auto AddVillain = [&](const FString& InId, const FString& InName, const FString& InRole,
        const FString& InDistrict, const FString& InThreat, const FString& InTrait, int32 InHP, int32 InDMG)
    {
        FTOHVillain Villain;
        Villain.Id = InId;
        Villain.Name = InName;
        Villain.Role = InRole;
        Villain.District = InDistrict;
        Villain.Threat = InThreat;
        Villain.Trait = InTrait;
        Villain.HP = InHP;
        Villain.DMG = InDMG;
        Villains.Add(Villain);
    };

    auto AddDistrict = [&](const FString& InId, const FString& InName, const FString& InDesc, const FString& InControl, const FString& InAtmosphere)
    {
        FTOHDistrict District;
        District.Id = InId;
        District.Name = InName;
        District.Description = InDesc;
        District.Control = InControl;
        District.Atmosphere = InAtmosphere;
        Districts.Add(District);
    };

    // HEROES
    AddHero(TEXT("lonzo"), TEXT("Lonzo Tech Specialist"), TEXT("Support"), TEXT("Cyber Arm"),
        TEXT("Pulse Burst"), TEXT("Shock Grid"), TEXT("Null Burst"),
        TEXT("The Other Half core tech healer and field specialist."), TEXT("Benway City"), TEXT("Hero"));

    AddHero(TEXT("carrie"), TEXT("Carrie Cyber Psionic"), TEXT("Controller"), TEXT("Energy Core"),
        TEXT("Mind Surge"), TEXT("Aether Grip"), TEXT("Core Sync"),
        TEXT("Carrie runs the red-cyan energy control line and protects the city core."), TEXT("Benway City"), TEXT("Hero"));

    // BOSS
    AddVillain(TEXT("warden"), TEXT("The Warden"), TEXT("Prison Boss"), TEXT("Prison District"), TEXT("Extreme"), TEXT("Security control"), 500, 70);

    // DISTRICTS
    AddDistrict(TEXT("prison"), TEXT("Prison District"), TEXT("Metal cells, security towers, and brutal control systems."), TEXT("The Warden"), TEXT("Harsh and oppressive"));
    AddDistrict(TEXT("benway"), TEXT("Benway City"), TEXT("The heart of the game world, a neon metropolis in desperate need of recovery."), TEXT("Operation Recover"), TEXT("Bright, unstable, and alive"));

    // OPTIONAL: keep these if you want a stronger base later
    // AddHero(TEXT("darrel"), TEXT("Darrel Kennel Boss"), TEXT("Tank"), TEXT("Blue Chest Core"), TEXT("Security Lock"), TEXT("Stomp Pulse"), TEXT("Keeper Wall"), TEXT("Security titan and front line defender."), TEXT("Kennel Row"), TEXT("Hero"));
    // AddHero(TEXT("adam"), TEXT("Adam Bunny Coins"), TEXT("Ranged"), TEXT("Cannon Burst"), TEXT("Bunny Dash"), TEXT("Coin Burst"), TEXT("Volt Shot"), TEXT("Fast-moving ranged specialist with a bunny-tech stance."), TEXT("Red Light Row"), TEXT("Hero"));
}

FTOHHero ATOHGameMode::GetHeroById(const FString& HeroId) const
{
    for (const FTOHHero& Hero : AllHeroes)
    {
        if (Hero.Id.Equals(HeroId, ESearchCase::IgnoreCase))
        {
            return Hero;
        }
    }

    return FTOHHero();
}

FTOHVillain ATOHGameMode::GetVillainById(const FString& VillainId) const
{
    for (const FTOHVillain& Villain : Villains)
    {
        if (Villain.Id.Equals(VillainId, ESearchCase::IgnoreCase))
        {
            return Villain;
        }
    }

    return FTOHVillain();
}

TArray<FTOHHero> ATOHGameMode::GetHeroRoster() const
{
    return AllHeroes;
}

TArray<FTOHDistrict> ATOHGameMode::GetDistricts() const
{
    return Districts;
}
