// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/UserDefinedEnum.h"
#include "TurtleBotTypeEnum.generated.h"

UENUM(BlueprintType) // Add this if the enum needs to be exposed to Blueprints
enum class TurtleBotTypeEnum : uint8 { // Use `uint8` for Unreal compatibility
	ONE UMETA(DisplayName = "One"),
	TWO UMETA(DisplayName = "Two"),
	THREE UMETA(DisplayName = "Three")
};