// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignatrue);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignatrue, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0"), Category = "Heal")
	float HealUpdateTime = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0"), Category = "Heal")
	float HealDelay = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0"), Category = "Heal")
	float HealModifier = 1.f;

	UFUNCTION(BlueprintCallable)
	bool isDead();

	USTUHealthComponent();
	float GetHealth() const;

	FOnDeathSignatrue OnDeath;
	FOnHealthChangedSignatrue OnHealthChanged;

protected:
	virtual void BeginPlay() override;

private:
	float Health{ 0.f };

	FTimerHandle TimerHandle;

	UFUNCTION()
	void OnTakeAnyDamageHandler(
	AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);
};
