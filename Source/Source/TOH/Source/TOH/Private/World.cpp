#include "World.h"

AWorldManager::AWorldManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AWorldManager::BeginPlay()
{
    Super::BeginPlay();
    LoadWorld();
}

void AWorldManager::LoadWorld()
{
    AllLocations.Empty();
    AllMissions.Empty();
    Facilities.Empty();

    auto AddLocation = [&](const FString& InId, const FString& InName, const FString& InDesc, 
        FVector InCoord, const FString& InType, const FString& InControlledBy, bool bActive, int32 InDifficulty)
    {
        FTOHLocation Loc;
        Loc.Id = InId;
        Loc.Name = InName;
        Loc.Description = InDesc;
        Loc.LocationCoord = InCoord;
        Loc.Type = InType;
        Loc.ControlledBy = InControlledBy;
        Loc.bIsActive = bActive;
        Loc.Difficulty = InDifficulty;
        AllLocations.Add(Loc);
    };

    auto AddMission = [&](const FString& InId, const FString& InTitle, const FString& InDesc,
        const FString& InLocationId, const FString& InBossId, int32 InReward, bool bCompleted, const FString& InObjectiveType)
    {
        FTOHMission Mission;
        Mission.Id = InId;
        Mission.Title = InTitle;
        Mission.Description = InDesc;
        Mission.LocationId = InLocationId;
        Mission.BossId = InBossId;
        Mission.Reward = InReward;
        Mission.bCompleted = bCompleted;
        Mission.ObjectiveType = InObjectiveType;
        AllMissions.Add(Mission);
    };

    // ========== BENWAY CITY CENTRAL HUB ==========
    // The heart of PULSE PLAIT facility - neon-soaked mega-structure
    AddLocation(TEXT("benway_plaza"), TEXT("Benway Central Plaza"), 
        TEXT("The glowing heart of PULSE PLAIT. Massive neon signs display The Other Half's mission. Blue and cyan light floods the plaza. Lonzo and Carrie's command center."),
        FVector(0, 0, 0), TEXT("HUB"), TEXT("Operation Recover"), true, 0);

    AddLocation(TEXT("benway_core"), TEXT("Benway Energy Core"), 
        TEXT("Carrie's domain. Massive blue energy reactor pulses with power. Red-cyan energy conduits run through crystalline towers. The AI brain network hums here."),
        FVector(200, 0, 100), TEXT("FACILITY"), TEXT("Carrie + Lonzo"), true, 2);

    AddLocation(TEXT("benway_armory"), TEXT("Benway Armory"), 
        TEXT("Lonzo's workshop. Rows of gatling gun parts, blue pulse ammunition crates, and cyber arm repair stations. Blueprints of weapons hang on walls."),
        FVector(-200, 0, 50), TEXT("FACILITY"), TEXT("Lonzo"), true, 1);

    // ========== PRISON DISTRICT - THE WARDEN'S FORTRESS ==========
    // Steel towers, brutal security, holding cells
    AddLocation(TEXT("prison_gate"), TEXT("Prison Gate - Warden's Entry"), 
        TEXT("Massive metal gates with blue security lights. Guard towers scan all movement. The air is cold and oppressive. Steel walls stretch endlessly upward."),
        FVector(500, 500, 0), TEXT("ENTRANCE"), TEXT("The Warden"), true, 3);

    AddLocation(TEXT("prison_main"), TEXT("Main Prison Block"), 
        TEXT("Rows of holding cells, each with glowing blue lock systems. Prisoners behind reinforced glass. Security cameras rotate endlessly. The Warden's voice echoes through speakers."),
        FVector(550, 550, 0), TEXT("COMBAT_ZONE"), TEXT("The Warden"), true, 4);

    AddLocation(TEXT("prison_tower"), TEXT("Warden's Control Tower"), 
        TEXT("A towering black spire at the prison's center. The Warden sits in a high-tech command center overlooking all cells. Red and blue holographic displays track every prisoner."),
        FVector(600, 500, 200), TEXT("BOSS_ARENA"), TEXT("The Warden"), true, 5);

    AddLocation(TEXT("prison_armory"), TEXT("Prison Armory"), 
        TEXT("Security weapons cache. Heavy armor plating, stun batons, containment equipment. The Warden's arsenal glows with menacing blue tech."),
        FVector(500, 450, 50), TEXT("FACILITY"), TEXT("The Warden"), true, 3);

    // ========== RED LIGHT ROW - NEON QUEEN'S TERRITORY ==========
    // Illegal trades, neon signs, dangerous streets
    AddLocation(TEXT("redlight_main"), TEXT("Red Light Row Main Street"), 
        TEXT("Neon signs flicker in pink, red, and magenta. Holographic dancers advertise illegal services. Smoke and steam rise from vents. The Neon Queen controls everything here."),
        FVector(-500, 500, 0), TEXT("DISTRICT"), TEXT("Neon Queen"), false, 2);

    AddLocation(TEXT("redlight_club"), TEXT("Neon Queen's Club"), 
        TEXT("A massive nightclub with pulsing lights and holographic displays. The Neon Queen holds court on a platform surrounded by laser shows. Music is deafening."),
        FVector(-550, 550, 0), TEXT("BOSS_ARENA"), TEXT("Neon Queen"), false, 4);

    // ========== SCRAPYARD FIELDS - RUST FATHER'S DOMAIN ==========
    // Rusted metal, salvage piles, industrial chaos
    AddLocation(TEXT("scrapyard_entrance"), TEXT("Scrapyard Fields Gate"), 
        TEXT("Massive piles of rusted metal, broken machinery, and industrial debris. The smell of rust and oil is overwhelming. Rusted robots move slowly among the piles."),
        FVector(500, -500, 0), TEXT("ENTRANCE"), TEXT("Rust Father"), false, 2);

    AddLocation(TEXT("scrapyard_core"), TEXT("Scrapyard Core - Rust Father's Lair"), 
        TEXT("A massive fortress built from scrap metal. The Rust Father sits on a throne of welded steel. Sparks fly from ongoing repairs. The ground shakes with heavy machinery."),
        FVector(600, -550, 50), TEXT("BOSS_ARENA"), TEXT("Rust Father"), false, 4);

    // ========== HERB FIELDS - THE CHEMIST'S CORRUPTED LAB ==========
    // Greenhouses, chemical vats, poisonous atmosphere
    AddLocation(TEXT("herb_fields_entrance"), TEXT("Herb Fields Entrance"), 
        TEXT("Beautiful green fields fade into a toxic haze. Greenhouses glow with unnatural light. The air tastes of chemicals. Vines hang from structures, some glowing, some wilting."),
        FVector(-500, -500, 0), TEXT("ENTRANCE"), TEXT("The Chemist"), false, 3);

    AddLocation(TEXT("herb_lab_core"), TEXT("Corrupt Lab Center"), 
        TEXT("Massive vats of glowing green liquid bubble ominously. The Chemist works at a central terminal surrounded by mutagenic plants. Mind-controlled workers shuffle around mindlessly."),
        FVector(-550, -550, 0), TEXT("BOSS_ARENA"), TEXT("The Chemist"), false, 5);

    // ========== KENNEL ROW - HOUND MASTER'S WARCAMP ==========
    // Military outpost, tracking beasts, armor
    AddLocation(TEXT("kennel_row_gate"), TEXT("Kennel Row Gate"), 
        TEXT("A militarized compound with watch towers and razor wire. Snarling beasts in containment cages. The Hound Master's flag flies high. Armor clad soldiers patrol."),
        FVector(0, 500, 0), TEXT("ENTRANCE"), TEXT("Hound Master"), false, 3);

    AddLocation(TEXT("kennel_beast_arena"), TEXT("Beast Training Arena"), 
        TEXT("A massive arena where the Hound Master's pack of mechanical beasts are trained. Claw marks scar the walls. The Hound Master stands on a command platform surrounded by snarling creatures."),
        FVector(50, 550, 0), TEXT("BOSS_ARENA"), TEXT("Hound Master"), false, 4);

    // ========== BLADE ALLEY - BLADE MOTHER'S DOMAIN ==========
    // Sharp architecture, deadly maze, precision strikes
    AddLocation(TEXT("blade_alley_entrance"), TEXT("Blade Alley Entrance"), 
        TEXT("Narrow streets lined with razor-sharp architecture. Metal blades protrude from walls. The air feels lethal. Every corner is an ambush point."),
        FVector(0, -500, 0), TEXT("ENTRANCE"), TEXT("Blade Mother"), false, 2);

    AddLocation(TEXT("blade_mother_sanctum"), TEXT("Blade Mother's Sanctum"), 
        TEXT("A maze of blades and mirrors. The Blade Mother moves with supernatural speed through the corridors. Her silhouette appears and disappears in reflections."),
        FVector(50, -550, 0), TEXT("BOSS_ARENA"), TEXT("Blade Mother"), false, 4);

    // ========== OVERLOOK - THE OVERLORD'S TOWER ==========
    // Control center, high-tech command, omnipotent surveillance
    AddLocation(TEXT("overlook_base"), TEXT("Overlook Base Level"), 
        TEXT("A massive elevator platform leads upward into endless sky. The air is thin and cold. Holographic displays show city-wide surveillance feeds."),
        FVector(-300, 0, 50), TEXT("ENTRANCE"), TEXT("The Overlord"), false, 4);

    AddLocation(TEXT("overlook_tower"), TEXT("The Overlord's Control Tower"), 
        TEXT("The highest point in PULSE PLAIT. The Overlord stands in a massive command center surrounded by holographic projections of the entire city. His presence is omnipotent."),
        FVector(-300, 0, 500), TEXT("BOSS_ARENA"), TEXT("The Overlord"), false, 6);

    // ========== MISSIONS ==========
    AddMission(TEXT("mission_warden"), TEXT("Prison Breach"), 
        TEXT("The Warden has taken control of the prison district. Lonzo and Carrie must infiltrate, bypass security, and confront The Warden in his control tower."),
        TEXT("prison_tower"), TEXT("warden"), 1000, false, TEXT("DEFEAT_BOSS"));

    AddMission(TEXT("mission_core_sync"), TEXT("Energy Core Defense"), 
        TEXT("Carrie's energy core is under attack. Lonzo must defend the core while Carrie syncs with the AI brain network to amplify her power."),
        TEXT("benway_core"), TEXT("none"), 500, false, TEXT("SURVIVE_WAVES"));

    AddMission(TEXT("mission_armory_raid"), TEXT("Armory Raid"), 
        TEXT("The Warden's forces are raiding Lonzo's armory. Lonzo must defend his weapons cache and salvage what he can before it's destroyed."),
        TEXT("benway_armory"), TEXT("none"), 300, false, TEXT("DEFEND_LOCATION"));
}

FTOHLocation AWorldManager::GetLocationById(const FString& LocationId) const
{
    for (const FTOHLocation& Loc : AllLocations)
    {
        if (Loc.Id.Equals(LocationId, ESearchCase::IgnoreCase))
        {
            return Loc;
        }
    }

    return FTOHLocation();
}

TArray<FTOHLocation> AWorldManager::GetLocationsByDistrict(const FString& District) const
{
    TArray<FTOHLocation> Result;
    for (const FTOHLocation& Loc : AllLocations)
    {
        if (Loc.ControlledBy.Equals(District, ESearchCase::IgnoreCase))
        {
            Result.Add(Loc);
        }
    }
    return Result;
}

FTOHMission AWorldManager::GetMissionById(const FString& MissionId) const
{
    for (const FTOHMission& Mission : AllMissions)
    {
        if (Mission.Id.Equals(MissionId, ESearchCase::IgnoreCase))
        {
            return Mission;
        }
    }

    return FTOHMission();
}

TArray<FTOHMission> AWorldManager::GetAvailableMissions() const
{
    TArray<FTOHMission> Result;
    for (const FTOHMission& Mission : AllMissions)
    {
        if (!Mission.bCompleted)
        {
            Result.Add(Mission);
        }
    }
    return Result;
}
