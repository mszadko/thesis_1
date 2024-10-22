// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "BaseCharacter.h"
#include "UnrealNetwork.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Skill.h"
#include "Runtime/Engine/Public/TimerManager.h"



//sets static names for axis binding (more info in header file)
const FName ABasePlayerController::LookUpBinding("LookUp");
const FName ABasePlayerController::LookRightBinding("LookRight");
const FName ABasePlayerController::MoveUpBinding("MoveUp");
const FName ABasePlayerController::MoveRightBinding("MoveRight");

ABasePlayerController::ABasePlayerController()
{
	SetReplicates(true);
	RotationSpeed = 0.1f;
	bIsLmbPressedDown = false;
}

void ABasePlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABasePlayerController, Skills);
	DOREPLIFETIME(ABasePlayerController, bIsUsingPad);
	
}

void ABasePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	Move(DeltaTime);
	if (bIsUsingRightAnalog)
		CallBasicAttack();
	
}
void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//=========JUMPING==========
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABasePlayerController::CallJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ABasePlayerController::CallStopJumping);

	//=========MOVING===========
	InputComponent->BindAxis(MoveUpBinding);
	InputComponent->BindAxis(MoveRightBinding);

	//=========ROTATING=========
	//(we don't want to call delegate functions each frame - we will handle it like man without built in functions)
	InputComponent->BindAxis(LookUpBinding);
	InputComponent->BindAxis(LookRightBinding);

	//=========SKILLS==========
	InputComponent->BindAction("Skill0", IE_Pressed, this, &ABasePlayerController::CallSkill0OnPress);
	InputComponent->BindAction("Skill1", IE_Pressed, this, &ABasePlayerController::CallSkill1OnPress);
	InputComponent->BindAction("Skill2", IE_Pressed, this, &ABasePlayerController::CallSkill2OnPress);
	InputComponent->BindAction("Skill3", IE_Pressed, this, &ABasePlayerController::CallSkill3OnPress);
	InputComponent->BindAction("Skill0", IE_Released, this, &ABasePlayerController::CallSkill0OnRelease);
	InputComponent->BindAction("Skill1", IE_Released, this, &ABasePlayerController::CallSkill1OnRelease);
	InputComponent->BindAction("Skill2", IE_Released, this, &ABasePlayerController::CallSkill2OnRelease);
	InputComponent->BindAction("Skill3", IE_Released, this, &ABasePlayerController::CallSkill3OnRelease);
	InputComponent->BindAction("Dodge", IE_Pressed, this, &ABasePlayerController::CallDodge);
	InputComponent->BindAction("BasicAttack", IE_Pressed, this, &ABasePlayerController::CallBasicAttack);
	InputComponent->BindAction("BasicAttack", IE_Released, this, &ABasePlayerController::ReleaseBasicAttack);
}

void ABasePlayerController::CallJump()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter,APawn>(GetPawn()))
		ControlledCharacter->Jump();
}

void ABasePlayerController::CallStopJumping()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter, APawn>(GetPawn()))
		ControlledCharacter->StopJumping();
}

void ABasePlayerController::Move(float DeltaTime)
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		//we have to read input values form the hardware
		float MoveHorizontalInputValue = GetInputAxisValue(MoveUpBinding);
		float MoveVerticalInputValue = GetInputAxisValue(MoveRightBinding);
		//then we apply it to a character
		ControlledCharacter->AddMovementInput(FVector(MoveHorizontalInputValue, MoveVerticalInputValue, 0.0f), DeltaTime*ControlledCharacter->GetMovementComponent()->GetMaxSpeed());
		if (bIsUsingPad)//if pad is used check what analog indicate direction
		{
			//we have to check if player is using right analog
			float RotateHorizontalInputValue = GetInputAxisValue(LookUpBinding);
			float RotateVerticalInputValue = GetInputAxisValue(LookRightBinding);
			if (FVector2D(RotateHorizontalInputValue, RotateVerticalInputValue).Size() > 0.25)//if right analog is used
			{
				bIsUsingRightAnalog = true;
				TurnToDirection(RotateHorizontalInputValue, RotateVerticalInputValue);
			}
			else
			{
				bIsUsingRightAnalog = false;
				TurnToDirection(MoveHorizontalInputValue, MoveVerticalInputValue);
			}
		}
		else //face cursor direction
		{
			RotateTowardsMouse();
		}
	}
}
//==================================ROTATING THE PLAYER=========================================
void ABasePlayerController::TurnToDirection(float HorizontalAxisValue, float VerticalAxisValue)
{
	if (ACharacter* ControlledCharacter = GetCharacter())
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
			SetControlRotation(InterpolatedRotation);
		}
	}
}

void ABasePlayerController::RotateTowardsMouse()
{
	if (ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter()))
	{
		//we basicaly want to block input so we don't want to rotate player towards cursor when character is dodging
		if (ControlledCharacter->bIsDodging)
			return;

		//this will store info where our cursor hits.
		FHitResult TraceHitResult;
		//in ue4 repo gethitresultundercursor is marked as deprecated so im using this one
		GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, TraceHitResult);
		FVector CharacterLocation = ControlledCharacter->GetActorLocation();
		FVector CursorLocation = TraceHitResult.Location;
		//calculates vector from character location to cursor location (we will use its direction later)
		FVector CharacterToCursor = CursorLocation - CharacterLocation;
		if (CharacterToCursor.Size() > 100.0f)//we don't want to rotate when cursor is on the player (it shakes the character)
		{
			//we will use this variable during interpolation from starter rotation to target rotation (so rotation can be nice and smooth)
			FRotator InterpolatedRotation = GetControlRotation();
			InterpolatedRotation = FMath::Lerp(InterpolatedRotation, CharacterToCursor.Rotation(), RotationSpeed);
			SetControlRotation(InterpolatedRotation);
		}
	}
}
//===============================END OF ROTATING THE PLAYER=========================================

void ABasePlayerController::CallDodge()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter,APawn>(GetPawn()))
		BC->Dodge(CalculateDodgeDirection());
}
void ABasePlayerController::CallSkill0OnPress()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
	{
		if (!(BC->bCanUseSkill0))
			return;
		if (BC->Skills[0]!=nullptr)
		{
			BC->SetIsCastingOnServer(true);
			BC->bCanUseSkill0 = false;
			GetWorldTimerManager().SetTimer(Cooldown0Timer, this, &ABasePlayerController::ClearSkill0Cooldown, BC->Skills[0]->Cooldown);
			BC->Skills[0]->OnPress(BC, this);
		}	
	}
}
void ABasePlayerController::CallSkill0OnRelease()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
	{
		if (BC->Skills[0] != nullptr)
		{
			BC->SetIsCastingOnServer(false);
			BC->Skills[0]->OnRelease(BC, this);
		}
	}
}
void ABasePlayerController::CallSkill1OnPress()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
	{
		if (!(BC->bCanUseSkill1))
			return;
		if (BC->Skills[1] != nullptr)
		{
			BC->SetIsCastingOnServer(true);
			BC->bCanUseSkill1 = false;
			GetWorldTimerManager().SetTimer(Cooldown1Timer, this, &ABasePlayerController::ClearSkill1Cooldown, BC->Skills[1]->Cooldown);
			BC->Skills[1]->OnPress(BC, this);
		}
	}
}
void ABasePlayerController::CallSkill1OnRelease()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
	{
		if (BC->Skills[1] != nullptr)
		{
			BC->SetIsCastingOnServer(false);
			BC->Skills[1]->OnRelease(BC, this);
		}
	}
}
void ABasePlayerController::CallSkill2OnPress()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
	{
		if (!(BC->bCanUseSkill2))
			return;
		if (BC->Skills[2] != nullptr)
		{
			BC->SetIsCastingOnServer(true);
			BC->bCanUseSkill2 = false;
			GetWorldTimerManager().SetTimer(Cooldown2Timer, this, &ABasePlayerController::ClearSkill2Cooldown, BC->Skills[2]->Cooldown);
			BC->Skills[2]->OnPress(BC, this);
		}
	}
}
void ABasePlayerController::CallSkill2OnRelease()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
	{
		if (BC->Skills[2] != nullptr)
		{
			BC->SetIsCastingOnServer(false);
			BC->Skills[2]->OnRelease(BC, this);
		}
	}
}
void ABasePlayerController::CallSkill3OnPress()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
	{
		if (!(BC->bCanUseSkill3))
			return;
		if (BC->Skills[3] != nullptr)
		{
			BC->SetIsCastingOnServer(true);
			BC->bCanUseSkill3 = false;
			GetWorldTimerManager().SetTimer(Cooldown3Timer, this, &ABasePlayerController::ClearSkill3Cooldown, BC->Skills[3]->Cooldown);
			BC->Skills[3]->OnPress(BC, this);
		}
	}
}
void ABasePlayerController::CallSkill3OnRelease()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
	{
		if (BC->Skills[3] != nullptr)
		{
			BC->SetIsCastingOnServer(false);
			BC->Skills[3]->OnRelease(BC, this);
		}
	}
}

void ABasePlayerController::CallBasicAttack()
{
	bIsLmbPressedDown = true;
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
		BC->BasicAttack();
}
void ABasePlayerController::ReleaseBasicAttack()
{
	bIsLmbPressedDown = false;
}

FVector ABasePlayerController::CalculateDodgeDirection()
{
	//read values form left analog (moving)
	float MoveHorizontalInputValue = GetInputAxisValue(MoveUpBinding);
	float MoveVerticalInputValue = GetInputAxisValue(MoveRightBinding);
	//create the dodge vector that will be passed to add movement method
	FVector DodgeVector = FVector(MoveHorizontalInputValue, MoveVerticalInputValue, 0.0f);
	//if we don't move we want to dash backwards
	if (FMath::IsNearlyZero(DodgeVector.Size()))
	{
		DodgeVector = -GetActorForwardVector();
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Input = (%f,%f,%f)"),DodgeVector.X,DodgeVector.Y,DodgeVector.Z));
	}
	else
	{
		DodgeVector = DodgeVector.GetUnsafeNormal();
	}
	return DodgeVector;
}

void ABasePlayerController::Disable()
{
	DisableInput(Cast<APlayerController>(this));
}

void ABasePlayerController::Enable()
{
	EnableInput(Cast<APlayerController>(this));
}

void ABasePlayerController::ClearSkill0Cooldown()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
		BC->bCanUseSkill0 = true;
}
void ABasePlayerController::ClearSkill1Cooldown()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
		BC->bCanUseSkill1 = true;
}
void ABasePlayerController::ClearSkill2Cooldown()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
		BC->bCanUseSkill2 = true;
}
void ABasePlayerController::ClearSkill3Cooldown()
{
	if (ABaseCharacter* BC = Cast<ABaseCharacter, APawn>(GetPawn()))
		BC->bCanUseSkill3 = true;
}