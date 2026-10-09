PublicDependencies.AddRange(new string[] { 
    "Core", 
    "CoreUObject", 
    "Engine", 
    "InputCore"
});

if (Target.Platform == UnrealTargetPlatform.Android)
{
    PublicDefinitions.Add("WITH_ANDROID_SUPPORT=1");
}
