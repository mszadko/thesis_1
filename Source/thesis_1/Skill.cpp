// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill.h"
#include "BaseCharacter.h"
#include "BasePlayerController.h"
#include "Engine.h"

ASkill::ASkill()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ASkill constructed"));
	}
}

void ASkill::OnPress_Implementation(ABaseCharacter* BaseCharacter, ABasePlayerController* PlayerController)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ASkill::OnPressed()"));
	}
}
void ASkill::OnRelease_Implementation(class ABaseCharacter* BaseCharacter, class ABasePlayerController* PlayerController)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ASkill::OnReleased()"));
	}
}



