// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurtleLidarBottom.generated.h"

UCLASS()
class TUBEUE_API ATurtleLidarBottom : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere) class UStaticMeshComponent* sBodyMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	ATurtleLidarBottom();
	virtual void Tick(float DeltaTime) override;
};
