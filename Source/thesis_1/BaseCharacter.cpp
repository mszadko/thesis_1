// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
//needed to print text on screen should be removed on later.
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
	CameraBoom->TargetArmLength = 400.f;
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
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//==========DASH============
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABaseCharacter::Dash);
	//====Basic attack(mouse)==
	PlayerInputComponent->BindAction("BasicAttack", IE_Pressed, this, &ABaseCharacter::BasicAttack);
}


//========================================SKILLS================================================
void ABaseCharacter::Dash_Implementation()
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector()*DashDistance, true);
}
void ABaseCharacter::BasicAttack()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BasicAttack"));
	}
}
