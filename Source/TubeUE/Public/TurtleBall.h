// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurtleBall.generated.h"

UCLASS()
class TUBEUE_API ATurtleBall : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "Components") class UStaticMeshComponent* sBodyMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	ATurtleBall();
	virtual void Tick(float DeltaTime) override;
};