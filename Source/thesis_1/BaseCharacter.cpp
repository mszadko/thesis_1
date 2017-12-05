// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "BasePlayerController.h"
#include "Skill.h"
#include "BaseCharacterMovementComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setting capsule size for collision
	GetCapsuleComponent()->InitCapsuleSize(40.f, 95.f);

	//============================CAMERA HOOK(BOOM)====================================
	//Create object.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//make boom child of RootComponent. now boom will inherit rotation and location of RootComponent of a character.
	CameraBoom->SetupAttachment(RootComponent);
	//Set how far the camera will be
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-55.0f,0.0f,0.0f));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	//==========================END OF CAMERA HOOK(BOOM)===============================

	//==========================CHARACTER CAMERA SETTING===============================
	PlayerCamera= CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	//Pin our camera to the camera hook.
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//=======================END OF CHARACTER CAMERA SETTING===========================
	

	DodgeDuration = 0.3f;
	DodgeCooldown = 0.5f;
	bCanDodge = true;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, Skills);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::LoadSkills()
{
	if (ABasePlayerController* BPC = Cast<ABasePlayerController, AController>(GetController()))
	{
		Skills.SetNumZeroed(4);
		//creating USkill objects based on USkill classes from BPC. couldnt use CreateDefaultSubobject as it doesn't accept a class type value as a parameter

		if (BPC->Skills.Num() == 4)
		{
			Skills[0] = BPC->Skills[0]->GetDefaultObject<USkill>();
			Skills[1] = BPC->Skills[1]->GetDefaultObject<USkill>();
			Skills[2] = BPC->Skills[2]->GetDefaultObject<USkill>();
			Skills[3] = BPC->Skills[3]->GetDefaultObject<USkill>();
		}
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//========================================SKILLS================================================
//===============Dodge================
void ABaseCharacter::Dodge(FVector CalculatedDodgeDirection)
{
	if (bCanDodge && !(GetMovementComponent()->IsFalling()))
	{
		bCanDodge = false;
		bIsDodging = true;
		if (ABasePlayerController* BPC = Cast<ABasePlayerController>(GetController()))
			BPC->Disable();
		UBaseCharacterMovementComponent* MoveComp = Cast<UBaseCharacterMovementComponent>(GetCharacterMovement());
		if (MoveComp)
			MoveComp->SetDodging(true);
		GetWorldTimerManager().SetTimer(DodgeDurationTimerHandle, this, &ABaseCharacter::FinishedDodge, DodgeDuration);
		GetWorldTimerManager().SetTimer(DodgeCooldownTimerHandle, this, &ABaseCharacter::AfterDodgeCooldown, DodgeCooldown);
		//LaunchCharacter(FVector(0.0f, 0.0f, 150.0f), false, false);//will make character jump a little bit on dash. if will be used we can add variable with DodgeJumpHeight
		//turn of colliders
		//set character so it can't recive damage
		//etc.
	}
}

void ABaseCharacter::FinishedDodge()
{
	bIsDodging = false;
	Cast<ABasePlayerController>(GetController())->Enable();
	UBaseCharacterMovementComponent* MoveComp = Cast<UBaseCharacterMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->SetDodging(false);
	}
	//turn on colliders
	//set character so it can recive dmg 
	//etc.
}
void ABaseCharacter::AfterDodgeCooldown()
{
	bCanDodge = true;
}

//=============End of Dodge================



void ABaseCharacter::BasicAttack()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseCharacter::BasicAttack"));
	}
}

