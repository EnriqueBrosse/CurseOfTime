// Fill out your copyright notice in the Description page of Project Settings.
// Made by Enrique Brosse

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TimeWarpComponent.h"
#include "../Components/PlayerAttackComponent.h"
#include "../Components/DashComponent.h"
#include "../Components/StaminaComponent.h"
#include "../components/KungFuGrid.h"
#include "../Actors/MeleeWeapon.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 0;
	GetCharacterMovement()->AirControl = 0;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// making the camera arm
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->bUsePawnControlRotation = true;
	// making the camera

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	TimeWarpComponent = CreateDefaultSubobject<UTimeWarpComponent>(TEXT("TimeWarpComponent"));

	PlayerAttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("UPlayerAttackComponent"));
	DashComponent = CreateDefaultSubobject<UDashComponent>(TEXT("DashComponent"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	KungFuGrid = CreateDefaultSubobject<UKungFuGrid>(TEXT("KungFuGrid"));
	KungFuGrid->SetupAttachment(RootComponent);

	Tags.Add("Player");
}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	 // later add the other components

	PlayerInputComponent->BindAction("TimeWarp", IE_Pressed, TimeWarpComponent, &UTimeWarpComponent::Reverse);
	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, PlayerAttackComponent, &UPlayerAttackComponent::OnLightAttack);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, PlayerAttackComponent, &UPlayerAttackComponent::OnHeavyAttack);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &APlayerCharacter::SwitchWeapon);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, DashComponent, &UDashComponent::Dash);
}

void APlayerCharacter::CheckDeath()
{
	if (HealthComponent->GetIsDead())
	{
		UGameplayStatics::OpenLevel(GetWorld(), "MainLevel");
	}
}

void APlayerCharacter::OnDeath()
{
	PlayerAttackComponent->SetIsAttacking(false);
	// add death animation 
	if (!DeathMontage->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::OnDeath DeathMontage is invalid"));
	}
	else
	{
		StopAnimMontage(); 
		float check = PlayAnimMontage(DeathMontage);

		if (check == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::OnDeath PlayAnimMontage is invalid"));
		}
		GetWorldTimerManager().SetTimer(DeathTimeHandle, this, &APlayerCharacter::CheckDeath, check - CutoffDeathMontage, false);
		// the above function doesn't seem to work // checkdeath never gets called so player never dies
	}
	
	if (DeathSound && AttenuationSettings && ConcurrencySettings)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation(),
			1.f, 1.f, 0.f, AttenuationSettings, ConcurrencySettings, this);
	}
	PlayerAttackComponent->GetCurrentWeapon()->TurnOffCollision();
}

void APlayerCharacter::OnStun()
{
	// play hit anim 
	if (!StunMontage->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::OnStun stunmontage is invalid"));
		return;
	}
	float check = PlayAnimMontage(StunMontage);
	if (check == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::OnStun PlayAnimMontage is invalid"));
	}
	PlayerAttackComponent->GetCurrentWeapon()->TurnOffCollision();

}

void APlayerCharacter::OnKnockBack(const FVector& knockbackValue)
{
	LaunchCharacter(knockbackValue, true, false);
}

void APlayerCharacter::OnHit()
{
	AController* controller = GetController();
	if (controller && ForceFeedback)
	{
		APlayerController* playerController = Cast<APlayerController>(controller);
		if (playerController)
		{
			FForceFeedbackParameters Params;
			Params.Tag = "";
			Params.bLooping = false;
			playerController->ClientPlayForceFeedback(ForceFeedback, Params);
		}
	}
	if (HitSound && AttenuationSettings && ConcurrencySettings)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), GetActorRotation(),
			1.f, 1.f, 0.f, AttenuationSettings, ConcurrencySettings, this);
	}
			
	if (!PlayerAttackComponent->GetIsAttacking() && hitMontage)
	{
		PlayerAttackComponent->GetCurrentWeapon()->TurnOffCollision();
		PlayAnimMontage(hitMontage);
	}
}

void APlayerCharacter::OnShieldHit()
{
}

void APlayerCharacter::SetWeapon(AMeleeWeapon* weapon)
{
	if (!weapon->IsValidLowLevelFast())
	{
		return;
	}
	FDetachmentTransformRules detachRules(EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, true);
	FAttachmentTransformRules attachmentRulesWeapon{ EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,true };
	AMeleeWeapon* currentWeapon = PlayerAttackComponent->GetCurrentWeapon();
	if (currentWeapon == Weapon1)
	{
		// detach the weapon and set it to the other weapons position
		Weapon1->DetachFromActor(detachRules);
		Weapon1->SetActorLocationAndRotation(weapon->GetActorLocation(), weapon->GetActorRotation());
		Weapon1->RemoveOwner();
		Weapon1 = weapon;
	}
	else
	{
		// detach the weapon and set it to the other weapons position
		Weapon2->DetachFromActor(detachRules);
		Weapon2->SetActorLocationAndRotation(weapon->GetActorLocation(), weapon->GetActorRotation());
		Weapon2->RemoveOwner();
		Weapon2 = weapon;
	}
	weapon->AttachToComponent(GetMesh(), attachmentRulesWeapon, SocketWeapon1);
	weapon->SetOwner(this);
	weapon->AddOwner();
	PlayerAttackComponent->SetCurrentWeapon(weapon);
}

void APlayerCharacter::SwitchWeapon()
{
	if (PlayerAttackComponent->GetIsAttacking() || HealthComponent->GetIsDead())
	{
		return;
	}
	if (SwitchMontage)
	{
		PlayAnimMontage(SwitchMontage, SwitchMontageSpeed);
	}
	else
	{
		SwitchWeaponAnimCall();
	}
}


void APlayerCharacter::SwitchWeaponAnimCall()
{
	FAttachmentTransformRules attachmentRulesWeapon{ EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,true };
	FDetachmentTransformRules detachRules(EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, true);

	if (PlayerAttackComponent->GetCurrentWeapon() == Weapon1)
	{
		PlayerAttackComponent->SetCurrentWeapon(Weapon2);

		Weapon1->DetachFromActor(detachRules);
		Weapon1->AttachToComponent(GetMesh(), attachmentRulesWeapon, SocketWeapon2);
		Weapon2->DetachFromActor(detachRules);
		Weapon2->AttachToComponent(GetMesh(), attachmentRulesWeapon, SocketWeapon1);

	}
	else if (PlayerAttackComponent->GetCurrentWeapon() == Weapon2)
	{
		PlayerAttackComponent->SetCurrentWeapon(Weapon1);

		Weapon1->DetachFromActor(detachRules);
		Weapon1->AttachToComponent(GetMesh(), attachmentRulesWeapon, SocketWeapon1);
		Weapon2->DetachFromActor(detachRules);
		Weapon2->AttachToComponent(GetMesh(), attachmentRulesWeapon, SocketWeapon2);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// spawning in the weapons and putting them in the right spot in the mesh
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Owner = this;
	FAttachmentTransformRules attachmentRulesWeapon{ EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,true };
	if (Weapon1TypeToSpawnOnBeginPlay != NULL)
	{
		FTransform tranformWeapon = GetMesh()->GetSocketTransform(SocketWeapon1, ERelativeTransformSpace::RTS_ParentBoneSpace);
		Weapon1 = GetWorld()->SpawnActor<AMeleeWeapon>(Weapon1TypeToSpawnOnBeginPlay, tranformWeapon, spawnParameters);
		Weapon1->AttachToComponent(GetMesh(), attachmentRulesWeapon, SocketWeapon1);
		Weapon1->AddOwner();
		PlayerAttackComponent->SetCurrentWeapon(Weapon1);
	}
	if (Weapon2TypeToSpawnOnBeginPlay != NULL)
	{
		FTransform tranformWeapon = GetMesh()->GetSocketTransform(SocketWeapon2, ERelativeTransformSpace::RTS_ParentBoneSpace);
		Weapon2 = GetWorld()->SpawnActor<AMeleeWeapon>(Weapon2TypeToSpawnOnBeginPlay, tranformWeapon, spawnParameters);
		Weapon2->AttachToComponent(GetMesh(), attachmentRulesWeapon, SocketWeapon2);
		Weapon2->AddOwner();
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APlayerCharacter::MoveForward(float Value)
{
	if (HealthComponent->GetIsDead())
	{
		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (HealthComponent->GetIsDead())
	{
		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


AMeleeWeapon* APlayerCharacter::GetCurrentWeapon()const
{ 
	return PlayerAttackComponent->GetCurrentWeapon();
}

AMeleeWeapon* APlayerCharacter::GetOtherWeapon()const
{ 
	return PlayerAttackComponent->GetCurrentWeapon() == Weapon1 ? Weapon2 : Weapon1; 
}
