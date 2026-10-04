#include "TheOtherHalfGameMode.h"
#include "Engine/Engine.h"
#include "HeroBase.h"
ATheOtherHalfGameMode::ATheOtherHalfGameMode(){ DefaultPawnClass=AHeroBase::StaticClass(); }
void ATheOtherHalfGameMode::LoadAllData()
{
    FHeroData Darrel; Darrel.Id="H01"; Darrel.Name="Darrel Kennel Boss"; Darrel.Role="Tank Keeper - 2 BIG Keepers BEHIND security"; Darrel.Tech="Lonzo Tech Blue chest core"; Darrel.Ability1="Keeper Command"; Darrel.Ability2="Sell Swarm"; Darrel.Ability3="Tank Howl"; Darrel.Unlock="Starter"; Darrel.Note="BIG=KEEPERS BEHIND SMALL=SELL";
    FUnderlingData D1; D1.Level="T1"; D1.Name="Small Pit Sell Old Build"; D1.Health=100; D1.Damage=15;
    FUnderlingData D2; D2.Level="T2"; D2.Name="Armored Pit Sell"; D2.Health=250; D2.Damage=35;
    FUnderlingData D3; D3.Level="T3"; D3.Name="BIG Tank Keeper Keeper Model 2 Behind Security"; D3.Health=800; D3.Damage=90; D3.Desc="Lonzo tech inside KEEPERS never sell";
    Darrel.Underlings={D1,D2,D3}; AllHeroes.Add(Darrel);
    FHeroData Lonzo; Lonzo.Id="H04"; Lonzo.Name="Lonzo Tech Specialist"; Lonzo.Role="Tech Ops Support YOU - THE OTHER HALF 1/2"; Lonzo.Tech="Cyber Arm Blue Lightning"; Lonzo.Ability1="Lightning Cannon"; Lonzo.Ability2="Drone Hack"; Lonzo.Ability3="System Overload"; Lonzo.Unlock="Always"; AllHeroes.Add(Lonzo);
    FHeroData Carrie; Carrie.Id="H05"; Carrie.Name="Carrie Cyber Psionic"; Carrie.Role="Psionic Assault Wife - THE OTHER HALF 2/2"; Carrie.Tech="RED ARMOR RED HELMET BLUE CORE BLUE EYES LOCKED photo6086343256974746643.jpeg"; Carrie.Ability1="Lightning Discharge"; Carrie.Ability2="Psionic Shield"; Carrie.Ability3="Neural Rift"; Carrie.Note="LOCKED MASTER DO NOT ALTER"; Carrie.Unlock="Starter CoOp";
    FUnderlingData C1; C1.Level="T1"; C1.Name="Psionic Echo"; C1.Health=110; C1.Damage=25;
    FUnderlingData C2; C2.Level="T2"; C2.Name="Psionic Shield Drone"; C2.Health=320; C2.Damage=30;
    FUnderlingData C3; C3.Level="T3"; C3.Name="Neural Rift Tank"; C3.Health=900; C3.Damage=110;
    Carrie.Underlings={C1,C2,C3}; AllHeroes.Add(Carrie);
    FHeroData BZ; BZ.Id="H06"; BZ.Name="BZ Beezy Herbs Specialist"; BZ.Role="Herb Support - Mind Controlled"; BZ.Backstory="Herb liquid swapped by Chemist mind control steals Carrie brain USB remembers but not why until lab test"; BZ.Tech="White coat green vials"; BZ.Ability1="Herb Toss Heal"; BZ.Ability2="Gas Cloud"; BZ.Ability3="Titan Brew"; BZ.Counterpart="The Chemist"; BZ.Unlock="Level 3"; AllHeroes.Add(BZ);
    FHeroData Adam; Adam.Id="H02"; Adam.Name="Adam Bunny Coins"; Adam.Role="Heavy"; AllHeroes.Add(Adam);
    FHeroData Deacon; Deacon.Id="H03"; Deacon.Name="Deacon Blade Expert"; Deacon.Role="Blade"; Deacon.Counterpart="Blade Mother"; AllHeroes.Add(Deacon);
    FHeroData Nate; Nate.Id="H07"; Nate.Name="Big Nate Easter Egg"; Nate.Role="Tank Basket"; Nate.Note="Jinx the Clown ally"; AllHeroes.Add(Nate);
    FHeroData Pmac; Pmac.Id="H08"; Pmac.Name="P-MAC Tech Ops"; Pmac.Role="Gunner vs Archivist"; Pmac.Counterpart="Archivist Signal Breaker"; AllHeroes.Add(Pmac);
    FDistrictData D; D.Id="D01"; D.Name="Prison District"; D.RealMissouriBase="Jeff City Penitentiary rubble"; D.Description="Concrete towers riot fences blue stun batons Warden patrols"; D.Boss="The Warden"; D.SizeKM=5; AllDistricts.Add(D);
    D.Id="D02"; D.Name="Red Light Row"; D.RealMissouriBase="St Louis Riverfront neon"; D.Description="Pink neon holo dolls spider drones Jinx the Clown bleeding laughing knows trap sells for herb"; D.Boss="Neon Queen"; D.SizeKM=4; AllDistricts.Add(D);
    D.Id="D03"; D.Name="Scrapyard Fields"; D.RealMissouriBase="KC rail yards"; D.Description="Mountains of scrap gear spiders junk golems Adam found bunny bot here"; D.Boss="Rust Father"; AllDistricts.Add(D);
    D.Id="D04"; D.Name="Herb Fields Corrupt Lab"; D.RealMissouriBase="Columbia agri labs overgrown"; D.Description="Overgrown herb fields green vats Chemist lab BZ enhancer liquid swapped for mind control green gas"; D.Boss="The Chemist"; AllDistricts.Add(D);
    D.Id="D05"; D.Name="Kennel Row"; D.RealMissouriBase="Columbia North Darrel turf"; D.Description="Dog kennels BIG Tank Keepers behind Darrel as security keepers with Lonzo tech small sell dogs in cages Hound Master underbelly below"; D.Boss="Hound Master"; AllDistricts.Add(D);
    D.Id="D06"; D.Name="Blade Alley"; D.RealMissouriBase="St Louis alley cult"; D.Description="Narrow blade cult alley Blade Mother blue blades shrine Deacon origin"; D.Boss="Blade Mother"; AllDistricts.Add(D);
    D.Id="D07"; D.Name="Easter Plains"; D.RealMissouriBase="Missouri farmland"; D.Description="Farmland Big Nate basket eggs bombs decoys robotic legs"; D.Boss="Hidden"; AllDistricts.Add(D);
    D.Id="D08"; D.Name="Signal Tower Overlord Tower"; D.RealMissouriBase="Benway City Center Columbia downtown tower ruins"; D.Description="Two towers final Overlord Tower orange orb Signal Tower bunker blue screens holds Carrie brain USB switch controls her P-MAC vs Archivist final"; D.Boss="Overlord + Archivist"; AllDistricts.Add(D);
    if(GEngine) GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Cyan,FString::Printf(TEXT("THE OTHER HALF LOADED %d HEROES %d DISTRICTS"),AllHeroes.Num(),AllDistricts.Num()));
}
void ATheOtherHalfGameMode::StartOperationRecover(){ LoadAllData(); if(GEngine) GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,TEXT("STORY: BZ herb liquid swapped -> mind controlled -> steals Carrie brain USB -> lab test -> RED ALERT bed -> trap switch controls Carrie -> OPERATION RECOVER")); }