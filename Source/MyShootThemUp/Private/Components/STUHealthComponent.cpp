// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUHealthComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

bool USTUHealthComponent::isDead() { return FMath::IsNearlyZero(Health); }

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

float USTUHealthComponent::GetHealth() const { return Health; }


void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		OwnerActor->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandler);
	}
}

void USTUHealthComponent::OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || isDead() || !GetWorld()) return;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	SetHealth(Health - Damage);
	OnHealthChanged.Broadcast(Health);

	if (isDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal && GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

	
}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);
	OnHealthChanged.Broadcast(Health);

	if (FMath::IsNearlyEqual(Health, MaxHealth))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}


