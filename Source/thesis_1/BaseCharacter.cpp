// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
//#include "Skill.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
#include "BasePlayerController.h"
#include "Engine.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
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

	DashDistance = 200.0f;
	RotationSpeed = 0.1f;

	//setting up default skill array
	//Skills.Add(CreateDefaultSubobject<USkill>(TEXT("Skill0")));
	//Skills.Add(CreateDefaultSubobject<USkill>(TEXT("Skill1")));
	//Skills.Add(CreateDefaultSubobject<USkill>(TEXT("Skill2")));
	//Skills.Add(CreateDefaultSubobject<USkill>(TEXT("Skill3")));
	TSubclassOf<USkill> skillclass0;
	TSubclassOf<USkill> skillclass1;
	TSubclassOf<USkill> skillclass2;
	TSubclassOf<USkill> skillclass3;
	Skills.Add(skillclass0);
	Skills.Add(skillclass1);
	Skills.Add(skillclass2);
	Skills.Add(skillclass3);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (ABasePlayerController* BPC = Cast<ABasePlayerController, AController>(GetController()))
	{
		Skills = BPC->Skills;
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//========================================SKILLS================================================
void ABaseCharacter::Dash_Implementation()
{
	//try to cache and cast player controller to BasePlayerController
	if (ABasePlayerController* BPC = Cast<ABasePlayerController, AController>(GetController()))
	{
		//read values form left analog (moving)
		float MoveHorizontalInputValue = BPC->GetInputAxisValue(BPC->MoveUpBinding);
		float MoveVerticalInputValue = BPC->GetInputAxisValue(BPC->MoveRightBinding);
		//creat the dash vector that will be addet to current location
		FVector DashVector = FVector(MoveHorizontalInputValue, MoveVerticalInputValue, 0.0f);
		//if we don't move we want to dash forward
		if (FMath::IsNearlyZero(DashVector.Size()))
		{
			DashVector = GetActorForwardVector()*DashDistance;
		}	
		else
		{
			DashVector = DashVector.GetUnsafeNormal()*DashDistance;
		}
		SetActorLocation(GetActorLocation() + DashVector, true);
	}
}
void ABaseCharacter::BasicAttack()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseCharacter::BasicAttack"));
	}
}

