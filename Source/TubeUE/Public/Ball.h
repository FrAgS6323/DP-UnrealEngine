// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "ForceComponent.h"
#include "Ball.generated.h"

UCLASS(BlueprintType, Blueprintable)
class TUBEUE_API ABall : public AActor{
	GENERATED_BODY()
	private:
		double r = 0.015;
		UPROPERTY(EditAnywhere) class UStaticMeshComponent* ballMesh;
	protected:
		virtual void BeginPlay() override;
	public:	
		ABall();
		//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UForceComponent* forceComponent;
		double getRadius();
		virtual void Tick(float DeltaTime) override;
};
