// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/Engine.h"

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Boss0LevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class THESIS_1_API ABoss0LevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ABoss0LevelScriptActor();
	void BeginPlay();
	
};
