#include "TOHCharacter.h"

ATOHCharacter::ATOHCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ATOHCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (HeroData.Name.Len() > 0)
    {
        Health = MaxHealth;
        GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Cyan,
            FString::Printf(TEXT("%s loaded: %s"), *HeroData.Name, *HeroData.Tech));
    }
}

void ATOHCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATOHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATOHCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATOHCharacter::MoveRight);

    PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &ATOHCharacter::PrimaryAttack);
    PlayerInputComponent->BindAction(TEXT("SecondaryAttack"), IE_Pressed, this, &ATOHCharacter::SecondaryAttack);
    PlayerInputComponent->BindAction(TEXT("Ability1"), IE_Pressed, this, &ATOHCharacter::Ability1);
    PlayerInputComponent->BindAction(TEXT("Ability2"), IE_Pressed, this, &ATOHCharacter::Ability2);
    PlayerInputComponent->BindAction(TEXT("Ability3"), IE_Pressed, this, &ATOHCharacter::Ability3);
}

void ATOHCharacter::SetHeroData(const FTOHHero& InHeroData)
{
    HeroData = InHeroData;
    MaxHealth = 100.f;
    Health = MaxHealth;
}

void ATOHCharacter::MoveForward(float Value)
{
    if (Value == 0.f) return;
    AddMovementInput(GetActorForwardVector(), Value);
}

void ATOHCharacter::MoveRight(float Value)
{
    if (Value == 0.f) return;
    AddMovementInput(GetActorRightVector(), Value);
}

void ATOHCharacter::PrimaryAttack()
{
    if (HeroData.Name.Len() == 0) return;

    if (HeroData.Id.Equals(TEXT("lonzo"), ESearchCase::IgnoreCase))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue,
            FString::Printf(TEXT("%s fires BLUE PULSE rounds from the Gatling gun arm!"), *HeroData.Name));
    }
    else if (HeroData.Id.Equals(TEXT("carrie"), ESearchCase::IgnoreCase))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple,
            FString::Printf(TEXT("%s launches telekinetic force through the AI-linked core!"), *HeroData.Name));
    }
}

void ATOHCharacter::SecondaryAttack()
{
    if (HeroData.Name.Len() == 0) return;

    if (HeroData.Id.Equals(TEXT("lonzo"), ESearchCase::IgnoreCase))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue,
            FString::Printf(TEXT("%s fires a charged pulse shock burst!"), *HeroData.Name));
    }
    else if (HeroData.Id.Equals(TEXT("carrie"), ESearchCase::IgnoreCase))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple,
            FString::Printf(TEXT("%s uses telekinetic crush to pull enemies inward!"), *HeroData.Name));
    }
}

void ATOHCharacter::Ability1()
{
    if (HeroData.Name.Len() == 0) return;

    if (HeroData.Id.Equals(TEXT("lonzo"), ESearchCase::IgnoreCase))
    {
        ActiveAbilityName = HeroData.A1;
        GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Cyan,
            FString::Printf(TEXT("%s activates %s: GATLING BARRAGE!"), *HeroData.Name, *HeroData.A1));
    }
    else if (HeroData.Id.Equals(TEXT("carrie"), ESearchCase::IgnoreCase))
    {
        ActiveAbilityName = HeroData.A1;
        GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Magenta,
            FString::Printf(TEXT("%s activates %s: TELEKINETIC CRUSH!"), *HeroData.Name, *HeroData.A1));
    }
}

void ATOHCharacter::Ability2()
{
    if (HeroData.Name.Len() == 0) return;

    if (HeroData.Id.Equals(TEXT("lonzo"), ESearchCase::IgnoreCase))
    {
        ActiveAbilityName = HeroData.A2;
        GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Cyan,
            FString::Printf(TEXT("%s activates %s: PULSE SHOCK!"), *HeroData.Name, *HeroData.A2));
    }
    else if (HeroData.Id.Equals(TEXT("carrie"), ESearchCase::IgnoreCase))
    {
        ActiveAbilityName = HeroData.A2;
        GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Magenta,
            FString::Printf(TEXT("%s activates %s: MENTAL OVERRIDE!"), *HeroData.Name, *HeroData.A2));
    }
}

void ATOHCharacter::Ability3()
{
    if (HeroData.Name.Len() == 0) return;

    if (HeroData.Id.Equals(TEXT("lonzo"), ESearchCase::IgnoreCase))
    {
        ActiveAbilityName = HeroData.A3;
        GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Cyan,
            FString::Printf(TEXT("%s activates %s: ARM SHIELD!"), *HeroData.Name, *HeroData.A3));
    }
    else if (HeroData.Id.Equals(TEXT("carrie"), ESearchCase::IgnoreCase))
    {
        ActiveAbilityName = HeroData.A3;
        GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Magenta,
            FString::Printf(TEXT("%s activates %s: CORE SYNC!"), *HeroData.Name, *HeroData.A3));
    }
}
