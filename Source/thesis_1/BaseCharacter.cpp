// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
//needed to print text on screen should be removed on later.
#include "Engine.h"

//sets static names for axis binding (more info in header file)
const FName ABaseCharacter::LookUpBinding("LookUp");
const FName ABaseCharacter::LookRightBinding("LookRight");
const FName ABaseCharacter::MoveUpBinding("MoveUp");
const FName ABaseCharacter::MoveRightBinding("MoveRight");


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
	Move();
	if (!bIsUsingPad)
	{
		RotateTowardsMouse();
	}
	//if player use right analog it should override rotation from left analog
	{
		float HorizontalFromRightAnalog = GetInputAxisValue(LookUpBinding);
		float VerticalFromRightAnalog = GetInputAxisValue(LookRightBinding);
		if (FVector2D(HorizontalFromRightAnalog,VerticalFromRightAnalog).Size()>0.25)
		{
			TurnToDirection(HorizontalFromRightAnalog, VerticalFromRightAnalog);
			BasicAttack();
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
	PlayerInputComponent->BindAxis(MoveUpBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);

	//=========ROTATING=========
	//(we don't want to call delegate functions each frame - we will handle it like man without built in functions)
	PlayerInputComponent->BindAxis(LookUpBinding);
	PlayerInputComponent->BindAxis(LookRightBinding);

	//==========DASH============
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABaseCharacter::Dash);
	//====Basic attack(mouse)==
	PlayerInputComponent->BindAction("BasicAttack", IE_Pressed, this, &ABaseCharacter::BasicAttack);
}

void ABaseCharacter::Move()
{
	//we have to read input values form the hardware
	float HorizontalInputValue = GetInputAxisValue(MoveUpBinding);
	float VerticalInputValue = GetInputAxisValue(MoveRightBinding);
	//then we apply it to a character
	AddMovementInput(FVector(HorizontalInputValue, VerticalInputValue, 0.0f));
	if (bIsUsingPad)
	{
		TurnToDirection(HorizontalInputValue, VerticalInputValue);
	}
}



//==================================ROTATING THE PLAYER=========================================
void ABaseCharacter::TurnToDirection(float HorizontalAxisValue,float VerticalAxisValue)
{
	const FVector Direction = FVector(HorizontalAxisValue, VerticalAxisValue, 0.0f);// .GetClampedToMaxSize(1.0f);
	FRotator InterpolatedRotation = GetControlRotation();
	if (Direction.Size() > 0.25)//if vector magnitude is greater than 0.25 we rotate (it prevents rotating to (0,0,0) if we release the analog)
	{
		//then we create rotator (basically it is vector(pitch,yaw,roll) with some extras)
		const FRotator TargetRotation = Direction.Rotation();
		//we don't want to rotate too hasty so we interpolate
		InterpolatedRotation = FMath::Lerp(InterpolatedRotation, TargetRotation, RotationSpeed);
		//and then we tell controller to rotate our character.
		GetController()->SetControlRotation(InterpolatedRotation);
	}
}

void ABaseCharacter::RotateTowardsMouse()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		//this will store info where our cursor hits.
		FHitResult TraceHitResult;
		//in ue4 repo gethitresultundercursor is marked as deprecated so im using this one
		PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, TraceHitResult);
		FVector CharacterLocation = GetActorLocation();
		FVector CursorLocation = TraceHitResult.Location;
		//calculates vector from character location to cursor location (we will use its direction later)
		FVector CharacterToCursor = CursorLocation - CharacterLocation;
		if (CharacterToCursor.Size() > 100.0f)//we don't want to rotate when cursor is on the player (it shakes the character)
		{
			//we will use this variable during interpolation from starter rotation to target rotation (so rotation can be nice and smooth)
			FRotator InterpolatedRotation = GetControlRotation();
			InterpolatedRotation = FMath::Lerp(InterpolatedRotation, CharacterToCursor.Rotation(), RotationSpeed);
			GetController()->SetControlRotation(InterpolatedRotation);
		}
	}
}
//===============================END OF ROTATING THE PLAYER=========================================


//========================================SKILLS================================================
void ABaseCharacter::Dash_Implementation()
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector()*DashDistance, true);
	DashSpecialEffect();
}
void ABaseCharacter::BasicAttack()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BasicAttack"));
	}
}
