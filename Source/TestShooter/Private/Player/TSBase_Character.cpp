// TestShooter. All rights reserved.

#include "Player/TSBase_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TSHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"

// Sets default values

ATSBase_Character::ATSBase_Character()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<UTSHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATSBase_Character::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ATSBase_Character::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ATSBase_Character::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ATSBase_Character::OnGroundLanded);
}

// Called every frame
void ATSBase_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATSBase_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ATSBase_Character::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ATSBase_Character::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ATSBase_Character::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ATSBase_Character::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATSBase_Character::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATSBase_Character::StartRun);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ATSBase_Character::StopRun);
}

void ATSBase_Character::MoveForward(float Amount)
{
    bIsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ATSBase_Character::MoveRight(float Amount)
{
    if (Amount == 0.0f)
        return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ATSBase_Character::StartRun()
{
    bWantsToRun = true;
    GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
}

void ATSBase_Character::StopRun()
{
    bWantsToRun = false;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

bool ATSBase_Character::IsRunning() const
{
    return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

float ATSBase_Character::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
        return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ATSBase_Character::OnDeath()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Character %s is dead"));
    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(5.0f);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ATSBase_Character::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ATSBase_Character::OnGroundLanded(const FHitResult& Hit) {
    const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::SanitizeFloat(FallVelocityZ));
    if (FallVelocityZ < LandedDamageVelocity.X)
        return;
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}