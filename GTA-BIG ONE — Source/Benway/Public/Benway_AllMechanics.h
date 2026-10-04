#pragma once
#include "CoreMinimal.h"
#include "Benway_AllMechanics.generated.h"

UCLASS() class UBenwayCore : public UObject {
GENERATED_BODY()
public:
float MapKM = 350.f; // bigger than GTA
TArray<FString> EvilFights = {"Warden Prison Bus Arena","Neon Bay Yacht Robbery","Scrap Desert Car Drop"};
void PlayRedAlert(){ UE_LOG(LogTemp, Warning, TEXT("Red Alert L5 - Lonzo: WAIT! Carrie faster, flies ahead, too late - hacked mid-flight"));}
void FlyCarrie(){ UE_LOG(LogTemp, Warning, TEXT("Carrie: flight + telekinesis + pulse shots from hands + AI brain"));}
void ControlledFight(){ UE_LOG(LogTemp, Warning, TEXT("You control Carrie 3 mins - fly/pulse/telekinesis - then hack takes over"));}
void BrainLayer(int L){ UE_LOG(LogTemp, Warning, TEXT("Inside brain Layer %d - reverse evil fight with BZ green vial + Deacon blue blades + robo-bunny"), L);}
void FreeBunny(){ UE_LOG(LogTemp, Warning, TEXT("Bunny cage - frees = 5s pause on hacked Carrie - no glitch empty space"));}
void Cage(){ UE_LOG(LogTemp, Warning, TEXT("Caged with YOUR stolen Pulse Plait tech - need 6 Big Nate Easter eggs to overload"));}
};