#include "Master.h"
AHero::AHero(){ PrimaryActorTick.bCanEverTick = false; }
void AHero::BeginPlay(){ Super::BeginPlay(); }
void AHero::Use1() {}
void AHero::Use2() {}
void AHero::Use3() {}
void AHero::Spawn(FString L) {}