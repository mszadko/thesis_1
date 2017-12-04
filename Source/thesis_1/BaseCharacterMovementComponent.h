// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

/**
*
*/
UCLASS()
class THESIS_1_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	typedef UCharacterMovementComponent Super;
	UBaseCharacterMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Unpack compressed flags from a saved move and set state accordingly. See FSavedMove_Character. */
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	/** Get prediction data for a client game. Should not be used if not running as a client.
	Allocates the data on demand and can be overridden to allocate a custom override if desired.
	Result must be a FNetworkPredictionData_Client_Character. */
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UPROPERTY(EditAnywhere, Category = "Dodge")
	float DodgeSpeedMultiplier;
	UPROPERTY(EditAnywhere, Category = "Dodge")
	float DodgeAccelerationMultiplier;
	void SetDodging(bool bDodging);
	uint8 bWantsToDodge : 1;
	virtual float GetMaxSpeed()const override;
	virtual float GetMaxAcceleration() const override;
	virtual FVector GetAirControl(float DeltaTime, float TickAirControl, const FVector& FallAcceleration) override;
	//friend class FSavedMove_ExtendedMyMovement;
private:
	FVector DodgeDirection;
};

class FSavedMove_BaseMovement : public FSavedMove_Character
{
public:

	typedef FSavedMove_Character Super;

	/**brief Resets all saved variables.*/
	virtual void Clear() override;

	/** @returns a byte containing encoded special movement information (jumping, crouching, etc.)	 */
	virtual uint8 GetCompressedFlags() const override;

	/** @Return true if this move can be combined with NewMove for replication without changing any behavior */
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

	/** Called to set up this saved move (when initially created) to make a predictive correction. (Sets up the move before sending it to the server.)*/
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
	/** Called before ClientUpdatePosition uses this SavedMove to make a predictive correction	 */
	virtual void PrepMoveFor(class ACharacter* Character) override;

	uint8 bSavedWantsToDodge : 1;
private:
};

class FNetworkPredictionData_Client_BaseMovement : public FNetworkPredictionData_Client_Character
{
public:
	FNetworkPredictionData_Client_BaseMovement(const UCharacterMovementComponent& ClientMovement);

	typedef FNetworkPredictionData_Client_Character Super;

	/** Allocate a new saved move. Subclasses should override this if they want to use a custom move class. */
	virtual FSavedMovePtr AllocateNewMove() override;
};