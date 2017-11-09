// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class THESIS_1_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	//code added to ACharacter
private:
	/*Camera boom*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	/*Camera that follows the player. (We have to think about whether the camera should be player property or there will be main camera that look at whole squad)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;
protected:
	/*Called for applying forward/backward input to character*/
	void MoveUp(float Value);

	/*Called for applying side to side input to character*/
	void MoveRight(float Value);
	
	/*Called for applying rotation to character (from mouse input)*/
	void RotateTowardsMouse(float Value);

	

public:

	//static names for axis bindings (Indicates that the InputComponent is interested in knowing the Axis value (via GetAxisValue) but does not want a delegate function called each frame.)
	//more information - https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Components/UInputComponent/BindAxis/index.html
	static const FName LookUpBinding;
	static const FName LookRightBinding;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return PlayerCamera; }
};
