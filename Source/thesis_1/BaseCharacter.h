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
	
	virtual void BasicAttack();

	//code added to ACharacter
private:
	/*Camera boom*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	/*Camera that follows the player. (We have to think about whether the camera should be player property or there will be main camera that look at whole squad)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;


	
public:

	//array of 4 skills that will be called when player press trigger or bumper
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
    TArray<class USkill*> Skills;

	/*Called when player press key to Dash. For more information about specifiers visit https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Reference/Functions/Specifiers/index.html */
	UFUNCTION(Server,Reliable,WithValidation)
	void Dash();
	void Dash_Implementation();

	//function needed when UFUNCTION got (WithValidation) tag
	virtual bool Dash_Validate() { return true; }

	//WARNINIG THOSE 2 PROPERTIES ARE NOW IN BASEPLAYERCONTROLLER. LEFT THOSE SO THE REST OF THE PROJECT WILL WORK ON MONDAY
	//Returns true if player is playing with pad, false if keyboard and mouse is used
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Settings)
	bool bIsUsingPad;
	//Determines how fast the interpolation of rotation should be([0;1] 1 means that interpolation will took only 1 step)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float RotationSpeed;

	//Determines how far dash will be.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float DashDistance;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return PlayerCamera; }
};

