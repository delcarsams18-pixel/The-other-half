#include "Hero.h"
#include "Engine/Engine.h"
AHero::AHero(){}
void AHero::BeginPlay(){ Super::BeginPlay(); if(GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Cyan,Data.Name); }
void AHero::Use1(){ if(GEngine) GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Blue,Data.A1); }
void AHero::Use2(){ if(GEngine) GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Green,Data.A2); }
void AHero::Use3(){ if(GEngine) GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,Data.A3); }
void AHero::Spawn(FString L){ for(auto &U:Data.Unders) if(U.Level==L) if(GEngine) GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Yellow,U.Name); }