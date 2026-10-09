#include "TOHCharacter.h"
#include "TOHProjectile.h"
#include "TOHGLBLoader.h"
#include "LonzoMeshData.h"
#include "ProceduralMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

ATOHCharacter::ATOHCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 450.0f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->SocketOffset = FVector(0.0f, 60.0f, 60.0f);

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    GetCharacterMovement()->bOrientRotationToMovement = false;

    ArmCannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmCannon"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CannonMesh(TEXT("/Engine/BasicShapes/Cylinder"));
    if (CannonMesh.Succeeded())
    {
        ArmCannon->SetStaticMesh(CannonMesh.Object);
        ArmCannon->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.8f));
        ArmCannon->SetRelativeLocation(FVector(30.0f, 25.0f, 90.0f));
        ArmCannon->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    }
    ArmCannon->SetupAttachment(GetMesh());

    GunBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunBody"));
    GunBarrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunBarrel"));
    GunGrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunGrip"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (BoxMesh.Succeeded())
    {
        GunBody->SetStaticMesh(BoxMesh.Object);
        GunBody->SetRelativeScale3D(FVector(0.15f, 0.5f, 0.25f));
        GunBody->SetRelativeLocation(FVector(35.0f, 30.0f, 100.0f));
        GunBody->SetupAttachment(GetMesh());

        GunBarrel->SetStaticMesh(CannonMesh.Object);
        GunBarrel->SetRelativeScale3D(FVector(0.08f, 0.08f, 0.6f));
        GunBarrel->SetRelativeLocation(FVector(35.0f, 30.0f, 105.0f));
        GunBarrel->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
        GunBarrel->SetupAttachment(GetMesh());

        GunGrip->SetStaticMesh(BoxMesh.Object);
        GunGrip->SetRelativeScale3D(FVector(0.12f, 0.15f, 0.35f));
        GunGrip->SetRelativeLocation(FVector(35.0f, 22.0f, 85.0f));
        GunGrip->SetRelativeRotation(FRotator(15.0f, 0.0f, 0.0f));
        GunGrip->SetupAttachment(GetMesh());
    }

    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (BodyMeshAsset.Succeeded())
    {
        BodyMesh->SetStaticMesh(BodyMeshAsset.Object);
        BodyMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.8f));
        BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    }
    BodyMesh->SetupAttachment(GetMesh());

    ProjectileClass = ATOHProjectile::StaticClass();
}

void ATOHCharacter::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;

    // Build Lonzo 3D model from embedded mesh data
    {
        UProceduralMeshComponent* LonzoModel = NewObject<UProceduralMeshComponent>(this);
        TArray<FVector> Vertices;
        TArray<int32> Triangles;
        TArray<FVector> Normals;
        TArray<FVector2D> UVs;
        TArray<FLinearColor> Colors;
        TArray<FProcMeshTangent> Tangents;
        
        int32 NumVerts = sizeof(Lonzo_Vertices) / sizeof(float) / 3;
        for (int32 i = 0; i < NumVerts; i++)
        {
            Vertices.Add(FVector(Lonzo_Vertices[i*3], Lonzo_Vertices[i*3+1], Lonzo_Vertices[i*3+2]));
            Normals.Add(FVector(0, 0, 1));
            UVs.Add(FVector2D(0, 0));
            Colors.Add(FLinearColor::White);
            Tangents.Add(FProcMeshTangent(1, 0, 0));
        }
        int32 NumIdx = sizeof(Lonzo_Indices) / sizeof(uint32);
        for (int32 i = 0; i < NumIdx; i++)
        {
            Triangles.Add((int32)Lonzo_Indices[i]);
        }
        LonzoModel->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
        LonzoModel->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        LonzoModel->SetRelativeLocation(FVector(0, 0, -90));
        LonzoModel->RegisterComponent();
        if (BodyMesh)
        {
            BodyMesh->SetVisibility(false);
        }
    }

    if (ArmCannon)
    {
        UMaterialInstanceDynamic* CannonMat = ArmCannon->CreateDynamicMaterialInstance(0);
        if (CannonMat)
        {
            CannonMat->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(0.05f, 0.1f, 0.15f));
            CannonMat->SetVectorParameterValue(TEXT("EmissiveColor"), FLinearColor(0.1f, 0.5f, 1.0f));
            CannonMat->SetScalarParameterValue(TEXT("EmissiveIntensity"), 3.0f);
        }
    }
}

void ATOHCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bTouchActive && IsAlive() && Controller)
    {
        APlayerController* PC = Cast<APlayerController>(Controller);
        if (PC)
        {
            int32 SX = 0, SY = 0;
            PC->GetViewportSize(SX, SY);
            if (TouchStart.X < SX * 0.5f)
            {
                float TX = 0.0f, TY = 0.0f;
                bool bPressed = false;
                PC->GetInputTouchState(TouchFinger, TX, TY, bPressed);
                FVector2D Delta = FVector2D(TX, TY) - FVector2D(TouchStart.X, TouchStart.Y);
                if (Delta.Size() > 20.0f)
                {
                    const FRotator YawRot(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
                    FVector Fwd = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
                    FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
                    AddMovementInput(Fwd, -Delta.Y / 200.0f);
                    AddMovementInput(Right, Delta.X / 200.0f);
                }
            }
            else
            {
                float TX = 0.0f, TY = 0.0f;
                bool bPressed = false;
                PC->GetInputTouchState(TouchFinger, TX, TY, bPressed);
                FVector2D Delta = FVector2D(TX, TY) - FVector2D(TouchStart.X, TouchStart.Y);
                AddControllerYawInput(Delta.X * 0.005f);
                AddControllerPitchInput(-Delta.Y * 0.005f);
                TouchStart = FVector(TX, TY, 0);
            }
        }
    }
}

void ATOHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATOHCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATOHCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATOHCharacter::LookUp);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATOHCharacter::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATOHCharacter::Fire);

    PlayerInputComponent->BindTouch(IE_Pressed, this, &ATOHCharacter::OnTouchPressed);
    PlayerInputComponent->BindTouch(IE_Released, this, &ATOHCharacter::OnTouchReleased);
}

void ATOHCharacter::OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
    TouchStart = Location;
    bTouchActive = true;
    TouchFinger = FingerIndex;
}

void ATOHCharacter::OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
    if (FingerIndex == TouchFinger)
    {
        bTouchActive = false;
        float TapDist = FVector::Dist(TouchStart, Location);
        if (TapDist < 30.0f)
        {
            Fire();
        }
    }
}

void ATOHCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f && IsAlive())
    {
        const FRotator YawRot(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        const FVector Dir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
        AddMovementInput(Dir, Value);
    }
}

void ATOHCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f && IsAlive())
    {
        const FRotator YawRot(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        const FVector Dir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
        AddMovementInput(Dir, Value);
    }
}

void ATOHCharacter::LookUp(float Value)
{
    if (Controller && Value != 0.0f && IsAlive())
    {
        AddControllerPitchInput(Value * 1.5f);
    }
}

void ATOHCharacter::Turn(float Value)
{
    if (Controller && Value != 0.0f && IsAlive())
    {
        AddControllerYawInput(Value * 1.5f);
    }
}

void ATOHCharacter::Fire()
{
    if (!IsAlive()) return;
    float Now = GetWorld()->GetTimeSeconds();
    if (Now - LastFireTime < FireCooldown) return;
    LastFireTime = Now;

    if (!ProjectileClass) return;

    FVector Muzzle = GetActorLocation() + GetActorForwardVector() * 80.0f + FVector(0, 0, 90);
    FRotator Dir = GetControlRotation();
    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = GetInstigator();
    GetWorld()->SpawnActor<ATOHProjectile>(ProjectileClass, Muzzle, Dir, Params);
}

void ATOHCharacter::ApplyDamage(float Amount)
{
    if (!IsAlive()) return;
    Health = FMath::Max(0.0f, Health - Amount);
    if (Health <= 0.0f)
    {
        GetCharacterMovement()->DisableMovement();
    }
}
