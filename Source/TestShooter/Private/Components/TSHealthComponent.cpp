// TestShooter. All rights reserved.

#include "Components/TSHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharLog, All, All);

UTSHealthComponent::UTSHealthComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void UTSHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UTSHealthComponent::OnTakeAnyDamage);
    }
}

void UTSHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    // UE_LOG(BaseCharLog, Display, TEXT("Damage: %f"), Damage);
    if (Damage <= 0.0f || IsDead() || !GetWorld())
        return;

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    SetHealth(FMath::Clamp(Health - Damage, 0.0f, MaxHealth));

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal && GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UTSHealthComponent::OnHeal, HealUpdateTime, true, DelayHeal);
    }
}

void UTSHealthComponent::OnHeal()
{
    SetHealth(Health + PointsHeal);
    OnHealthChanged.Broadcast(Health);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UTSHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}
