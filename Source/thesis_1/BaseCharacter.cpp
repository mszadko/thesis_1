// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

//sets static names for axis binding (more info in header file)
const FName ABaseCharacter::LookUpBinding("LookUp");
const FName ABaseCharacter::LookRightBinding("LookRight");


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setting capsule size for collision
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.f);

	//============================CAMERA HOOK(BOOM)====================================
	//Create object.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//make boom child of RootComponent. now boom will inherit rotation and location of RootComponent of a character.
	CameraBoom->SetupAttachment(RootComponent);
	//Set how far the camera will be
	CameraBoom->TargetArmLength = 300.f;
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
	//==========rotating with pad==================
	{
		//first we create vector with out input data and normalize it
		const FVector Direction = FVector(GetInputAxisValue(LookUpBinding), GetInputAxisValue(LookRightBinding), 0.0f).GetClampedToMaxSize(1.0f);
		if (Direction.Size()>0.25)//if size is bigger than 1/4 we rotate (otherwise character always face up when no input was given)
		{
			//then we create rotator (basically it is vector(roll,pitch,yaw) with some extras)
			const FRotator NewRotation = Direction.Rotation();
			//here i need to slow it down a little bit
			
			//and then we tell controller to rotate our character.
			GetController()->SetControlRotation(NewRotation);
		}

	}

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//=========JUMPING==========
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//=========MOVING===========
	PlayerInputComponent->BindAxis("MoveUp", this, &ABaseCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	//=========ROTATING=========
	//(it is different than moving becouse we don't want to call delegate functions each frame)
	PlayerInputComponent->BindAxis(LookUpBinding);
	PlayerInputComponent->BindAxis(LookRightBinding);

}


//============MOVEMENT==============
void ABaseCharacter::MoveUp(float InputValue)
{
	AddMovementInput(FVector(1.f, 0, 0), InputValue);
}

void ABaseCharacter::MoveRight(float InputValue)
{
	AddMovementInput(FVector(0, 1.f, 0), InputValue);
}
//=========END OF MOVEMENT==========