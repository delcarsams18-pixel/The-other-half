#include "TOHAndroid.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#endif

void FTOHAndroid::InitializeAndroid()
{
#if PLATFORM_ANDROID
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Android initialized for The Other Half"));
#endif
}

bool FTOHAndroid::IsAndroidDevice()
{
#if PLATFORM_ANDROID
    return true;
#else
    return false;
#endif
}

void FTOHAndroid::SetupTouchInput()
{
#if PLATFORM_ANDROID
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Touch input setup for Android"));
#endif
}
