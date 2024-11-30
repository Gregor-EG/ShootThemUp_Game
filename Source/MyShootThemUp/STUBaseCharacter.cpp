// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "MyShootThemUp/Public/Components/STUCharacterMovementComponent.h"

const float RunningSpeed = 600.f;
const float WalkingSpeed = 450.f;
const float IdlingSpeed = 0.f;


float ASTUBaseCharacter::GetMovementDirection() const
{
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}

float ASTUBaseCharacter::GetSpeed() const
{
	return GetVelocity().Size();
}

bool ASTUBaseCharacter::isRunning() const
{
	return WantsToRun && MovingForward && !GetVelocity().IsZero();
}

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit) : 
	Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);


}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASTUBaseCharacter::MoveForward(float Amount)
{
	if (CharacterMovementComponent->IsFalling()) return;
	if (AnimInstance->GetCurrentStateName(0) == "EndJump") return;


	MovingForward = Amount > 0.f;


	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if (CharacterMovementComponent->IsFalling()) return;
	if (AnimInstance->GetCurrentStateName(0) == "EndJump") return;

	AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::StartRunning()
{
	WantsToRun = true;
}

void ASTUBaseCharacter::StopRunning()
{
	WantsToRun = false;
}

void ASTUBaseCharacter::UpdateMaxSpeed(float DeltaTime)
{
	float CurrentMaxSpeed = CharacterMovementComponent->MaxWalkSpeed;
	if (TargetSpeed == CurrentMaxSpeed) return;

	float InterpolatedSpeed = FMath::FInterpTo(CurrentMaxSpeed, TargetSpeed, DeltaTime, 10.f);
	CharacterMovementComponent->MaxWalkSpeed = InterpolatedSpeed;
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UpdateMaxSpeed(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	CharacterMovementComponent = GetCharacterMovement();
	SkeletalMeshComponent = GetMesh();
	AnimInstance = SkeletalMeshComponent->GetAnimInstance();

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookAround", this, &ASTUBaseCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::StopRunning);
}

