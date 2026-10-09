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
    AddHero(TEXT("darrel"), TEXT("Darrel Kennel Boss"), TEXT("Tank"), TEXT("Blue Chest Core"),
        TEXT("Security Lock"), TEXT("Stomp Pulse"), TEXT("Keeper Wall"),
        TEXT("Security titan and front line defender."), TEXT("Kennel Row"), TEXT("Hero"));

    AddHero(TEXT("adam"), TEXT("Adam Bunny Coins"), TEXT("Ranged"), TEXT("Cannon Burst"),
        TEXT("Bunny Dash"), TEXT("Coin Burst"), TEXT("Volt Shot"),
        TEXT("Fast-moving ranged specialist with a bunny-tech stance."), TEXT("Red Light Row"), TEXT("Hero"));

    AddHero(TEXT("deacon"), TEXT("Deacon Blade Expert"), TEXT("Assassin"), TEXT("Dual Blades"),
        TEXT("Blade Dash"), TEXT("Cutter Arc"), TEXT("Shadow Step"),
        TEXT("Fast melee specialist built for quick takedowns."), TEXT("Blade Alley"), TEXT("Hero"));

    AddHero(TEXT("lonzo"), TEXT("Lonzo Tech Specialist"), TEXT("Support"), TEXT("Cyber Arm"),
        TEXT("Pulse Burst"), TEXT("Shock Grid"), TEXT("Null Burst"),
        TEXT("The Other Half core tech healer and field specialist."), TEXT("Benway City"), TEXT("Hero"));

    AddHero(TEXT("carrie"), TEXT("Carrie Cyber Psionic"), TEXT("Controller"), TEXT("Energy Core"),
        TEXT("Mind Surge"), TEXT("Aether Grip"), TEXT("Core Sync"),
        TEXT("Carrie runs the red-cyan energy control line and protects the city core."), TEXT("Benway City"), TEXT("Hero"));

    AddHero(TEXT("bz"), TEXT("BZ Beezy Herbs Specialist"), TEXT("Support"), TEXT("Herb Vials"),
        TEXT("Green Pulse"), TEXT("Patch Bloom"), TEXT("Regen Mist"),
        TEXT("Herb support and field medic for soft recovery."), TEXT("Herb Fields"), TEXT("Hero"));

    AddHero(TEXT("nate"), TEXT("Big Nate Easter Egg"), TEXT("Bruiser"), TEXT("Egg Basket"),
        TEXT("Bounce Slam"), TEXT("Egg Rain"), TEXT("Shell Bash"),
        TEXT("A chaotic bruiser who fights with surprising impact."), TEXT("Scrapyard Fields"), TEXT("Hero"));

    AddHero(TEXT("pmac"), TEXT("P-MAC Tech Ops"), TEXT("Drone Control"), TEXT("Drone Arm"),
        TEXT("Drone Sweep"), TEXT("Signal Jam"), TEXT("Overclock"),
        TEXT("Drone operator and tactical support specialist."), TEXT("Prison District"), TEXT("Hero"));

    // VILLAINS
    AddVillain(TEXT("warden"), TEXT("The Warden"), TEXT("Prison Boss"), TEXT("Prison District"), TEXT("Extreme"), TEXT("Security control"), 500, 70);
    AddVillain(TEXT("neonqueen"), TEXT("Neon Queen"), TEXT("Control"), TEXT("Red Light Row"), TEXT("High"), TEXT("Light manipulation"), 420, 65);
    AddVillain(TEXT("rustfather"), TEXT("Rust Father"), TEXT("Scrap Titan"), TEXT("Scrapyard Fields"), TEXT("High"), TEXT("Heavy armor"), 470, 60);
    AddVillain(TEXT("chemist"), TEXT("The Chemist"), TEXT("Mutant Lab"), TEXT("Herb Fields"), TEXT("Critical"), TEXT("Poison + corruption"), 600, 90);
    AddVillain(TEXT("houndmaster"), TEXT("Hound Master"), TEXT("Dog Pack"), TEXT("Kennel Row"), TEXT("High"), TEXT("Alpha pack"), 430, 58);
    AddVillain(TEXT("blademother"), TEXT("Blade Mother"), TEXT("Assassin Mother"), TEXT("Blade Alley"), TEXT("High"), TEXT("Precision strike"), 440, 66);
    AddVillain(TEXT("overlord"), TEXT("The Overlord"), TEXT("Mastermind"), TEXT("Benway City"), TEXT("Extreme"), TEXT("Command and control"), 700, 95);
    AddVillain(TEXT("archivist"), TEXT("The Archivist"), TEXT("Signal Keeper"), TEXT("Prison District"), TEXT("High"), TEXT("Data corruption"), 390, 52);

    // DISTRICTS
    AddDistrict(TEXT("prison"), TEXT("Prison District"), TEXT("Metal cells, security towers, and brutal control systems."), TEXT("The Warden"), TEXT("Harsh and oppressive"));
    AddDistrict(TEXT("redlight"), TEXT("Red Light Row"), TEXT("Neon streets, illegal trades, and vulnerable citizens."), TEXT("Neon Queen"), TEXT("Hot and dangerous"));
    AddDistrict(TEXT("scrapyard"), TEXT("Scrapyard Fields"), TEXT("A ruined industrial zone full of scavenged power and danger."), TEXT("Rust Father"), TEXT("Rusty and chaotic"));
    AddDistrict(TEXT("herb"), TEXT("Herb Fields"), TEXT("Greenhouses, lairs, and a chemical war zone."), TEXT("The Chemist"), TEXT("Toxic but alive"));
    AddDistrict(TEXT("kennel"), TEXT("Kennel Row"), TEXT("A brutal warcamp of keepers, tracking beasts, and makeshift armor."), TEXT("Hound Master"), TEXT("Aggressive and militarized"));
    AddDistrict(TEXT("blade"), TEXT("Blade Alley"), TEXT("A sharp, violent maze where every corridor is a duel."), TEXT("Blade Mother"), TEXT("Fast and lethal"));
    AddDistrict(TEXT("benway"), TEXT("Benway City"), TEXT("The heart of the game world, a neon metropolis in desperate need of recovery."), TEXT("The Overlord"), TEXT("Bright, unstable, and alive"));
    AddDistrict(TEXT("overlook"), TEXT("Overlook"), TEXT("The high control towers above the city, where the city controller watches all."), TEXT("The Overlord"), TEXT("Cold and dominating"));
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
