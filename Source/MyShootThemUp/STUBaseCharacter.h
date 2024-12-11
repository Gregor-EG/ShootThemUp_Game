// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUHealthComponent;
class UTextRenderComponent;
class UCameraComponent;
class USpringArmComponent;
class UAnimInstance;

UCLASS()
class MYSHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Amination")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamageVelocity{900.f, 1200.f};

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamage{ 10.f, 100.f };

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetSpeed() const;

	UFUNCTION(BlueprintCallable)
	bool isRunning() const;

	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	virtual void BeginPlay() override;

private:
	UAnimInstance* AnimInstance;

	bool MovingForward = false;
	bool WantsToRun = false;

	float TargetSpeed{};
	float InterpolationTime{};

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void StartRunning();
	void StopRunning();

	void OnDeath();
	void OnHealthChanged(float Health);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
};
