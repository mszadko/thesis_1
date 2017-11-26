// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss0LevelScriptActor.h"

ABoss0LevelScriptActor::ABoss0LevelScriptActor()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Yellow, TEXT("Some debug message!"));
}


