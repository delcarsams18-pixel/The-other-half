USTRUCT(BlueprintType)
struct FUnderlingData { Level Name Type Desc Health Damage };
USTRUCT(BlueprintType)
struct FDistrictData { Id Name RealMissouriBase Description Visual Boss DangerLevel Loot SizeKM };
USTRUCT(BlueprintType)
struct FHeroData { Id Name Role Backstory Tech Ability1 Ability2 Ability3 Counterpart Unlock Note Underlings[] };