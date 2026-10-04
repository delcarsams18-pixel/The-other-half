#include "HeroBase.h"
#include "Engine/Engine.h"
AHeroBase::AHeroBase(){ PrimaryActorTick.bCanEverTick=true; }
void AHeroBase::BeginPlay(){ Super::BeginPlay(); if(GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Cyan,FString::Printf(TEXT("HERO LOADED: %s - %s | %s"),*HeroData.Id,*HeroData.Name,*HeroData.Role)); }
void AHeroBase::UseAbility1(){ if(GEngine) GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Blue,HeroData.Ability1); }
void AHeroBase::UseAbility2(){ if(GEngine) GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Green,HeroData.Ability2); }
void AHeroBase::UseAbility3(){ if(GEngine) GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,HeroData.Ability3); }
void AHeroBase::SpawnUnderling(FString Level){ for(auto &U:HeroData.Underlings){ if(U.Level==Level){ if(GEngine) GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Yellow,FString::Printf(TEXT("SPAWN %s %s HP:%d DMG:%d"),*U.Level,*U.Name,U.Health,U.Damage)); } } }