#include "TOHGameMode.h"
#include "Hero.h"
#include "Engine/Engine.h"
ATOHGameMode::ATOHGameMode(){ DefaultPawnClass=AHero::StaticClass(); }
void ATOHGameMode::Load()
{
    FHero D; D.Id="H01"; D.Name="Darrel Kennel Boss"; D.Role="Tank Keeper 2 BIG Behind Security KEEPERS Small SELL"; D.Tech="Lonzo Tech Blue chest core"; D.A1="Keeper Command"; D.A2="Sell Swarm"; D.A3="Tank Howl"; D.Note="BIG=KEEPERS BEHIND SECURITY SMALL=SELL EXACTLY AS IS";
    FUnder u1; u1.Level="T1"; u1.Name="Small Pit Sell Old Build"; u1.HP=100; u1.DMG=15; u1.Desc="Pre-Lonzo tech weak sell";
    FUnder u2; u2.Level="T2"; u2.Name="Armored Pit Sell"; u2.HP=250; u2.DMG=35;
    FUnder u3; u3.Level="T3"; u3.Name="BIG Tank Keeper 2 Behind Security"; u3.HP=800; u3.DMG=90; u3.Desc="Lonzo tech inside BLUE CORE KEEPERS never sell EXACTLY AS IS";
    D.Unders={u1,u2,u3}; AllHeroes.Add(D);

    FHero L; L.Id="H04"; L.Name="Lonzo Tech Specialist"; L.Role="YOU THE OTHER HALF 1/2"; L.Tech="Cyber Arm Blue Lightning"; L.A1="Lightning Cannon"; L.A2="Drone Hack"; L.A3="System Overload"; AllHeroes.Add(L);

    FHero C; C.Id="H05"; C.Name="Carrie Cyber Psionic"; C.Role="Wife THE OTHER HALF 2/2"; C.Tech="RED ARMOR RED HELMET BLUE CORE BLUE EYES LOCKED"; C.A1="Lightning Discharge"; C.A2="Psionic Shield"; C.A3="Neural Rift"; C.Note="LOCKED MASTER photo6086343256974746643.jpeg EXACTLY AS IS"; AllHeroes.Add(C);

    FHero B; B.Id="H06"; B.Name="BZ Beezy Herbs Specialist"; B.Role="Herb Support Mind Controlled"; B.Tech="White coat green vials green gauntlet"; B.A1="Herb Toss Heal"; B.A2="Gas Cloud"; B.A3="Titan Brew"; B.Note="Herb liquid swapped by Chemist mind control steals Carrie brain USB remembers but not why until lab test RED ALERT bed trap switch controls Carrie"; AllHeroes.Add(B);

    FHero A; A.Id="H02"; A.Name="Adam Bunny Coins"; A.Role="Heavy"; A.Tech="Bunny Bot Drone"; AllHeroes.Add(A);
    FHero De; De.Id="H03"; De.Name="Deacon Blade Expert"; De.Role="Blade Skull Shirt Dual Blue Blades"; De.Note="Counter Blade Mother"; AllHeroes.Add(De);
    FHero Na; Na.Id="H07"; Na.Name="Big Nate Easter Egg"; Na.Role="Tank Basket Pink Shorts Robotic Legs"; Na.Note="Jinx the Clown ally in Red Light Row bleeding laughing knows trap sells for herb"; AllHeroes.Add(Na);
    FHero Pm; Pm.Id="H08"; Pm.Name="P-MAC Tech Ops"; Pm.Role="Gunner vs Archivist Signal Breaker"; Pm.Note="Minigun Dossier Final Tower holds Carrie USB switch"; AllHeroes.Add(Pm);

    if(GEngine) GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Cyan,FString::Printf(TEXT("THE OTHER HALF LOADED %d HEROES EXACTLY AS IS - 5.5.4 FIXED"),AllHeroes.Num()));
}