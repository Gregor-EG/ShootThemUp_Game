// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyShootThemUp/Public/Components/STUCharacterMovementComponent.h"
#include "MyShootThemUp/Public/Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All)

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

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

	AnimInstance = GetMesh()->GetAnimInstance();
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
	if (GetCharacterMovement()->IsFalling()) return;
	if (AnimInstance->GetCurrentStateName(1) == "EndJump") return;

	MovingForward = Amount > 0.f;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if (GetCharacterMovement()->IsFalling()) return;
	if (AnimInstance->GetCurrentStateName(1) == "EndJump") return;

	AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookAround", this, &ASTUBaseCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::StopRunning);
}


void ASTUBaseCharacter::StartRunning()
{
	WantsToRun = true;
}

void ASTUBaseCharacter::StopRunning()
{
	WantsToRun = false;
}

void ASTUBaseCharacter::OnDeath()
{
	PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocity = -GetCharacterMovement()->Velocity.Z;
	if (FallVelocity < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocity);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}




