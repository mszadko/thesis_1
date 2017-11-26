// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill.h"
#include "BaseCharacter.h"
#include "BasePlayerController.h"
#include "Engine.h"

void USkill::OnPress(ABaseCharacter* BaseCharacter, ABasePlayerController* PlayerController)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("USkill::OnPressed()"));
	}
}
void USkill::OnRelease(class ABaseCharacter* BaseCharacter, class ABasePlayerController* PlayerController)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("USkill::OnReleased()"));
	}
}



