using UnrealBuildTool;

public class TOH : ModuleRules
{
    public TOH(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "ImageWrapper",
            "RenderCore",
            "RHI",
            "ProceduralMeshComponent",
            "Json",
            "JsonUtilities"
        });

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicDefinitions.Add("WITH_ANDROID_SUPPORT=1");
            PublicDefinitions.Add("PLATFORM_ANDROID=1");
            PrivateDependencyModuleNames.Add("AndroidPermission");
        }
    }
}