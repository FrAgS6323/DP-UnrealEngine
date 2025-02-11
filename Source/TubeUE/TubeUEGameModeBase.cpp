// Copyright Epic Games, Inc. All Rights Reserved.

#include "TubeUEGameModeBase.h"

void ATubeUEGameModeBase::InitGameState(){
	Super::InitGameState();

	this->turtlePawn = ATurtleBody::StaticClass();

	if(DefaultPawnClass == ADefaultPawn::StaticClass())
		DefaultPawnClass = this->turtlePawn;
}