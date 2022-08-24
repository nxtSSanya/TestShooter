// TestShooter. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Components/ActorComponent.h"
#include "TSHealthComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

    UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) class TESTSHOOTER_API UTSHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UTSHealthComponent();
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;
    FTimerHandle HealTimerHandle;

protected:
    // Called when the game starts
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float DelayHeal = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float PointsHeal = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.1", ClampMax = "5.0"))
    float HealUpdateTime = 0.3f;

    virtual void BeginPlay() override;

private:
    float Health = 0.0f;

    UFUNCTION()
    void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void OnHeal();
    void SetHealth(float NewHealth);
};
