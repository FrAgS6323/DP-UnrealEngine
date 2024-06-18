// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ForceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUBEUE_API UForceComponent : public USceneComponent{
	
	GENERATED_BODY()
	private:
		UPROPERTY()
		UStaticMeshComponent *meshComp;

		UPROPERTY(EditAnywhere, Category = "Force")
		float force = 1.0f;
	protected:
		// Called when the game starts
		virtual void BeginPlay() override;
	public:
		// Sets default values for this component's properties
		UForceComponent();
		// Called every frame
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
