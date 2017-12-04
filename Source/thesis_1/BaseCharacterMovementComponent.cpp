// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacterMovementComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "BasePlayerController.h"

UBaseCharacterMovementComponent::UBaseCharacterMovementComponent()
{
	DodgeSpeedMultiplier = 1.5f;
	DodgeAccelerationMultiplier = 10.0f;
}
void UBaseCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bWantsToDodge)
	{
		
		if (ACharacter* CharOwner = GetCharacterOwner())
		{
			CharOwner->AddMovementInput(DodgeDirection, DeltaTime*GetMaxSpeed());
		}
	}
}
void UBaseCharacterMovementComponent::SetDodging(bool bDodging)
{
	if (ABasePlayerController *BPC = Cast<ABasePlayerController>(GetCharacterOwner()->GetController()))
	{
		DodgeDirection = BPC->CalculateDodgeDirection();
	}
	bWantsToDodge = bDodging;
}
float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	if (bWantsToDodge)
	{
		MaxSpeed *= DodgeSpeedMultiplier;
	}
	return MaxSpeed;
}
float UBaseCharacterMovementComponent::GetMaxAcceleration() const
{
	float MaxAccel = Super::GetMaxAcceleration();
	if (bWantsToDodge)
	{
		MaxAccel *= DodgeAccelerationMultiplier;
	}
	return MaxAccel;
}
FVector UBaseCharacterMovementComponent::GetAirControl(float DeltaTime, float TickAirControl, const FVector& FallAcceleration)
{
	// Boost
	if (bWantsToDodge)
	{
		TickAirControl = BoostAirControl(DeltaTime, 1.0f, FallAcceleration);
	}
	else if (TickAirControl != 0.f)
	{
		TickAirControl = BoostAirControl(DeltaTime, TickAirControl, FallAcceleration);
	}

	return TickAirControl * FallAcceleration;
}
void UBaseCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bWantsToDodge = (Flags&FSavedMove_Character::FLAG_Custom_0) != 0;

}
class FNetworkPredictionData_Client* UBaseCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		UBaseCharacterMovementComponent* MutableThis = const_cast<UBaseCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_BaseMovement(*this);
	}
	return ClientPredictionData;
}
void FSavedMove_BaseMovement::Clear()
{
	Super::Clear();
	//here i should set all variables to default values
	bSavedWantsToDodge = false;
}


uint8 FSavedMove_BaseMovement::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	//here i should OR result with my new flags
	/*example i got FLAG_FollowingTheDamnTrain and bWantsToFollowTrain
	if(bWantsToFollowTrain)
	{
	result |= FLAG_FollowingTheDamnTrain
	}*/
	if (bSavedWantsToDodge)
	{
		Result |= FLAG_Custom_0;//becouse bSavedWantsToDodge uses first cutom flag
	}
	return Result;
}

bool FSavedMove_BaseMovement::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	if (bSavedWantsToDodge != ((FSavedMove_BaseMovement*)&NewMove)->bSavedWantsToDodge)
	{
		return false;
	}
	else
	{
		return Super::CanCombineWith(NewMove, Character, MaxDelta);
	}
}

void FSavedMove_BaseMovement::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UBaseCharacterMovementComponent* CharMov = Cast<UBaseCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{

		bSavedWantsToDodge = CharMov->bWantsToDodge;
	}
}

void FSavedMove_BaseMovement::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UBaseCharacterMovementComponent* CharMov = Cast<UBaseCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{

	}
}

FNetworkPredictionData_Client_BaseMovement::FNetworkPredictionData_Client_BaseMovement(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr FNetworkPredictionData_Client_BaseMovement::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_BaseMovement());
}