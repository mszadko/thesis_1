// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill.generated.h"

/**
 * 
 */
UCLASS()
class THESIS_1_API USkill : public UObject
{
	GENERATED_BODY()

public:
	/*will be called when player press the button. it can do whole logic for the skill or prepare character do use one. 
	(for ex. it can block character movement so the pawn wont move when we cast the spell
	or it can draw a line so the player can aim)
	*/
	virtual void OnPress(class ABaseCharacter* BaseCharacter, class ABasePlayerController* PlayerController);
	/*will be called when player release the button. it can be left blank if the whole logic is made in on press or it
	can finish what've been started in OnPress
	(for ex. it can release casted spell, allow character to move again, and hide aim line)*/
	virtual void OnRelease(class ABaseCharacter* BaseCharacter,class ABasePlayerController* PlayerController);
};
