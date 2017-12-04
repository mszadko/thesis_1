// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Skill.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THESIS_1_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

	//static names for axis bindings (Indicates that the InputComponent is interested in knowing the Axis value (via GetAxisValue) but does not want a delegate function called each frame.)
	//more information - https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Components/UInputComponent/BindAxis/index.html
	static const FName LookUpBinding;
	static const FName LookRightBinding;
	static const FName MoveUpBinding;
	static const FName MoveRightBinding;

	//Returns true if player is playing with pad, false if keyboard and mouse is used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	bool bIsUsingPad = true;

	//this will indicate if we want to get rotation from left or right analog
	bool bIsUsingRightAnalog = false;

	//Determines how fast the interpolation of rotation should be([0;1] 1 means that interpolation will took only 1 step)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float RotationSpeed;

	/*a character will load its skills from this array.
	it has to be populated from blueprints, because playerinfo is a bp structure and it can't be referenced in cpp
	(that's also why we can't read skills from playerinfo directly to a character if we wanted to do this in cpp)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TArray<TSubclassOf<USkill>> Skills;

private:
	bool bIsLmbPressedDown;

protected:
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

private:
	/*this will make character look in the direction indicated by the axis values (called form move to look forward and some attacks so druids can shoot from wands in right directions (or whack an axe if you are a friend of a druid)*/
	void TurnToDirection(float HorizontalAxisValue, float VerticalAxisValue);
	/*Called for applying rotation to character (from mouse input)*/
	void RotateTowardsMouse();
	/*Called for applying forward/backward & side to side input to character. This also checks whether rotation should be inherited form left or right analog and call RotateToDirection with proper parameters*/
	void Move(float DeltaTime);
	/*Called when right analog is used. It will face the player in the proper direction and do call basic attack function form controlled character*/
	void CallBasicAttack();

	void ReleaseBasicAttack();

	//Those are going to call Jump and StopJumping from PlayerMovementComponent of controlled character
	void CallJump();
	void CallStopJumping();

	void CallDodge();
	void CallSkill0OnPress();
	void CallSkill1OnPress();
	void CallSkill2OnPress();
	void CallSkill3OnPress();

	void CallSkill0OnRelease();
	void CallSkill1OnRelease();
	void CallSkill2OnRelease();
	void CallSkill3OnRelease();

public:
	void Disable();
	void Enable();
	/*@Returns FVector with values that should be added to movement to Dodge*/
	FVector CalculateDodgeDirection();
};
