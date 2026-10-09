ATOHGameMode::ATOHGameMode()
{
    DefaultPawnClass = ATOHCharacter::StaticClass();
    
    #if PLATFORM_ANDROID
    PlayerControllerClass = ATOHTouchController::StaticClass();
    #else
    PlayerControllerClass = ATOHPlayerController::StaticClass();
    #endif
}
