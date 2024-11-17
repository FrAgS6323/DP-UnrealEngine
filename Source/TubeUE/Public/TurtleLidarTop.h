// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurtleLidarTop.generated.h"

UCLASS()
class TUBEUE_API ATurtleLidarTop : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere) class UStaticMeshComponent* sBodyMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	ATurtleLidarTop();
	virtual void Tick(float DeltaTime) override;
};
