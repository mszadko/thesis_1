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
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	//needed for skills replication
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

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

	/*Function called by timer after dodge duration. It will enable input and clean all stuff that Dodge() did.*/
	UFUNCTION()
	void FinishedDodge();
	/*Timer handle that will be set with FinishedDodge function*/
	FTimerHandle DodgeDurationTimerHandle;
	/*Function called by timer after the dodge cooldown will time out. It will set character ready to another dash.*/
	UFUNCTION()
	void AfterDodgeCooldown();
	FTimerHandle DodgeCooldownTimerHandle;
	//indicates whether dodge cooldown timed out and we can do next one.
	bool bCanDodge;
public:
	//indicates whether we are in the middle of dodging. ( i think it may be helpful with making state machine for animation behavior)
	UPROPERTY(Replicated,BlueprintReadOnly)
	bool bIsDodging;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsCasting;

	UFUNCTION(Server, Reliable, WithValidation)
	void SetIsDodgingOnServer(bool NewIsDodging);
	void SetIsDodgingOnServer_Implementation(bool NewIsDodging);
	bool SetIsDodgingOnServer_Validate(bool NewIsDodging) { return true; }

	UFUNCTION(Server, Reliable, WithValidation)
	void SetIsCastingOnServer(bool NewIsCasting);
	void SetIsCastingOnServer_Implementation(bool NewIsCasting);
	bool SetIsCastingOnServer_Validate(bool NewIsCasting) { return true; }
	void Dodge(FVector CalculatedDodgeDirection);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
	float DodgeCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
	float DodgeDuration;

	//array of 4 skills that will be called when player press trigger or bumper
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Skills)
    TArray<class USkill*> Skills;

	UFUNCTION(BlueprintCallable)
	void LoadSkills();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return PlayerCamera; }
};

