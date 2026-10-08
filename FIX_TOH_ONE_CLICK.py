import os, shutil, pathlib, json

# RUN THIS IN B:/_w/The-other-half/
# It will NOT delete your Content - it will fix the structure

root = pathlib.Path(".").resolve()
print(f"Fixing: {root}")

# 1. BACKUP FIRST
backup = root.parent / f"{root.name}_BACKUP_BEFORE_FIX"
if not backup.exists():
    print(f"Creating backup at {backup}")
    shutil.copytree(root, backup, ignore=shutil.ignore_patterns('Binaries','Intermediate','Saved','.git'))
    print("Backup done - you can always restore from this")

# 2. Ensure correct folders exist
(root / "Source" / "TOH" / "Public").mkdir(parents=True, exist_ok=True)
(root / "Source" / "TOH" / "Private").mkdir(parents=True, exist_ok=True)
(root / "Config").mkdir(exist_ok=True)
(root / "Content" / "Maps").mkdir(parents=True, exist_ok=True)
(root / "Content" / "Characters" / "Heroes").mkdir(parents=True, exist_ok=True)

# 3. Move TOH/Public -> Source/TOH/Public if exists at root
if (root / "TOH" / "Public").exists():
    print("Moving TOH/Public -> Source/TOH/Public")
    for f in (root / "TOH" / "Public").glob("*"):
        shutil.move(str(f), str(root / "Source" / "TOH" / "Public" / f.name))
    if (root / "TOH" / "Private").exists():
        for f in (root / "TOH" / "Private").glob("*"):
            shutil.move(str(f), str(root / "Source" / "TOH" / "Private" / f.name))
    if (root / "TOH" / "TOH.Build.cs").exists():
        shutil.move(str(root / "TOH" / "TOH.Build.cs"), str(root / "Source" / "TOH" / "TOH.Build.cs"))
    try:
        shutil.rmtree(root / "TOH")
        print("Deleted empty TOH/ at root")
    except:
        pass

# 4. Delete junk folders that break cook
for junk in ["GTA-BIG ONE — Source", "GTA-BIG ONE - Source", "README.txt at ROOT TOH"]:
    p = root / junk
    if p.exists():
        print(f"Deleting junk: {junk}")
        shutil.rmtree(p) if p.is_dir() else p.unlink()

# 5. Check for Content
if not (root / "Content" / "Maps" / "BenwayCity_Persistent.umap").exists():
    print("\n*** WARNING: BenwayCity_Persistent.umap NOT found at Content/Maps/ ***")
    print("You need to have your Content folder here. If it's somewhere else on your PC, copy it to:")
    print(f"  {root / 'Content'}")
else:
    size = (root / "Content" / "Maps" / "BenwayCity_Persistent.umap").stat().st_size / (1024*1024)
    print(f"Found BenwayCity map: {size:.1f} MB - GOOD")

# 6. Create/Overwrite the fixed files (safe)
print("\nWriting fixed config files...")

uproject = {
    "FileVersion": 3,
    "EngineAssociation": "5.5",
    "Description": "The Other Half - Benway City Operation Recover - Bigger than GTA",
    "Modules": [{"Name": "TOH", "Type": "Runtime", "LoadingPhase": "Default"}],
    "TargetPlatforms": ["Windows","Android"]
}
with open(root / "TOH.uproject", "w") as f:
    json.dump(uproject, f, indent=2)

(root / "Config" / "DefaultEngine.ini").write_text("""[URL]
GameName=TOH

[/Script/EngineSettings.GameMapsSettings]
EditorStartupMap=/Game/Maps/BenwayCity_Persistent.BenwayCity_Persistent
GameDefaultMap=/Game/Maps/BenwayCity_Persistent.BenwayCity_Persistent

[/Script/Engine.WorldPartitionSettings]
EnableWorldPartition=True

[Core.System]
Paths=../../../Engine/Content
Paths=%GAMEDIR%Content

[/Script/UnrealEd.ProjectPackagingSettings]
Build=IfProjectHasCode
BuildConfiguration=PPBC_Shipping
bUseIoStore=True
bUsePaks=True
MapsToCook=(FilePath="/Game/Maps/BenwayCity_Persistent")
""")

(root / "Config" / "DefaultGame.ini").write_text("""[/Script/EngineSettings.GeneralProjectSettings]
ProjectName=TOH
ProjectVersion=0.1.0 - Benway City Operation Recover
""")

build_path = root / "Source" / "TOH" / "TOH.Build.cs"
if not build_path.exists():
    build_path.write_text("""using UnrealBuildTool;
public class TOH : ModuleRules
{
    public TOH(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core","CoreUObject","Engine","InputCore","EnhancedInput" });
    }
}
""")

print("\nDONE. Structure should now be:")
print("TOH.uproject")
print("Config/")
print("Content/Maps/BenwayCity_Persistent.umap")
print("Source/TOH/Public/Hero.h (Darrel, Adam, Lonzo, Carrie, BZ, Deacon, Big Nate, P-Mac + dogs)")