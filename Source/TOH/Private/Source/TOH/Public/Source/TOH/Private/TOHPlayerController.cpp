#include "TOHPlayerController.h"
#include "Hero.h"

ATOHPlayerController::ATOHPlayerController()
{
    bShowMouseCursor = false;
}

void ATOHPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void ATOHPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAxis(TEXT("MoveForward"), this, &ATOHPlayerController::MoveForward);
    InputComponent->BindAxis(TEXT("MoveRight"), this, &ATOHPlayerController::MoveRight);

    InputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &ATOHPlayerController::PrimaryAttack);
    InputComponent->BindAction(TEXT("SecondaryAttack"), IE_Pressed, this, &ATOHPlayerController::SecondaryAttack);
    InputComponent->BindAction(TEXT("Ability1"), IE_Pressed, this, &ATOHPlayerController::Ability1);
    InputComponent->BindAction(TEXT("Ability2"), IE_Pressed, this, &ATOHPlayerController::Ability2);
    InputComponent->BindAction(TEXT("Ability3"), IE_Pressed, this, &ATOHPlayerController::Ability3);
}

void ATOHPlayerController::MoveForward(float Value)
{
    ATOHCharacter* Character = Cast<ATOHCharacter>(GetPawn());
    if (!Character) return;

    if (Value != 0.f)
    {
        Character->AddMovementInput(Character->GetActorForwardVector(), Value);
    }
}

void ATOHPlayerController::MoveRight(float Value)
{
    ATOHCharacter* Character = Cast<ATOHCharacter>(GetPawn());
    if (!Character) return;

    if (Value != 0.f)
    {
        Character->AddMovementInput(Character->GetActorRightVector(), Value);
    }
}

void ATOHPlayerController::PrimaryAttack()
{
    ATOHCharacter* Character = Cast<ATOHCharacter>(GetPawn());
    if (!Character) return;

    Character->PrimaryAttack();
}

void ATOHPlayerController::SecondaryAttack()
{
    ATOHCharacter* Character = Cast<ATOHCharacter>(GetPawn());
    if (!Character) return;

    Character->SecondaryAttack();
}

void ATOHPlayerController::Ability1()
{
    ATOHCharacter* Character = Cast<ATOHCharacter>(GetPawn());
    if (!Character) return;

    Character->Ability1();
}

void ATOHPlayerController::Ability2()
{
    ATOHCharacter* Character = Cast<ATOHCharacter>(GetPawn());
    if (!Character) return;

    Character->Ability2();
}

void ATOHPlayerController::Ability3()
{
    ATOHCharacter* Character = Cast<ATOHCharacter>(GetPawn());
    if (!Character) return;

    Character->Ability3();
}