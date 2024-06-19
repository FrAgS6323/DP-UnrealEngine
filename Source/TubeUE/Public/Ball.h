// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForceComponent.h"
#include "Ball.generated.h"

UCLASS(BlueprintType, Blueprintable)
class TUBEUE_API ABall : public AActor{
	GENERATED_BODY()
	private:
		UPROPERTY(EditAnywhere)
		class UStaticMeshComponent *ballMesh;
	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	public:	
		// Sets default values for this actor's properties
		ABall();
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UForceComponent *forceComponent;
		// Called every frame
		virtual void Tick(float DeltaTime) override;
};
