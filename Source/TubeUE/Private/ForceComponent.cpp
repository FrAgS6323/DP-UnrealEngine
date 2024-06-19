// Fill out your copyright notice in the Description page of Project Settings.


#include "ForceComponent.h"

// Sets default values for this component's properties
UForceComponent::UForceComponent(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UForceComponent::BeginPlay(){
	Super::BeginPlay();
	this->meshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
}

// Called every frame
void UForceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const FVector forceDirUp = this->GetUpVector();

	if (this->meshComp) {
		UE_LOG(LogTemp, Warning, TEXT("Force: = %f"), this->force);
		//UE_LOG(LogTemp, Warning, TEXT("Ball mass: %f"), this->meshComp->GetMass());

		//forceDirUp * this->force * this->meshComp->GetMass()
		this->meshComp->AddForce(FVector(0, 0, this->force));
	}
}

