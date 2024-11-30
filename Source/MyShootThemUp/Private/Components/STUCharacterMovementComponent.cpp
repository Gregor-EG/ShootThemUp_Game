// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShootThemUp/Public/Components/STUCharacterMovementComponent.h"
#include "MyShootThemUp/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());

	return (Player && Player->isRunning()) ? MaxSpeed * RunModifier : MaxSpeed;
}
