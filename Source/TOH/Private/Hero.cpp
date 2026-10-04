#include "Hero.h"

AHero::AHero()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AHero::BeginPlay()
{
    Super::BeginPlay();
}

void AHero::Use1()
{
    // D.Name etc if needed
    if (!Data.A1.IsEmpty())
    {
        // placeholder
    }
}

void AHero::Use2()
{
    if (!Data.A2.IsEmpty())
    {
    }
}

void AHero::Use3()
{
    if (!Data.A3.IsEmpty())
    {
    }
}

void AHero::Spawn(FString L)
{
    // Spawn logic for Underlings level L
}