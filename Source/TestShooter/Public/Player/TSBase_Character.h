// TestShooter. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TSBase_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCharacterMovementComponent;
class UTSHealthComponent;
class UTextRenderComponent;

UCLASS()
class TESTSHOOTER_API ATSBase_Character : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ATSBase_Character();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCharacterMovementComponent* CharacterMovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTSHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);
    
    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

        // Called when the game starts or when spawned
        virtual void
        BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:
    bool bWantsToRun = false;
    bool bIsMovingForward = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void StartRun();
    void StopRun();
    void OnDeath();
    void OnHealthChanged(float Health);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
