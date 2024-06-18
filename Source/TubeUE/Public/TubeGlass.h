// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TubeGlass.generated.h"

UCLASS()
class TUBEUE_API ATubeGlass : public AActor{
	
	GENERATED_BODY()
	private:
		UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* tubeGlassMesh;
	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	public:
		// Sets default values for this actor's properties
		ATubeGlass();
		// Called every frame
		virtual void Tick(float DeltaTime) override;
};
