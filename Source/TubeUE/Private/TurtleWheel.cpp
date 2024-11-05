// Fill out your copyright notice in the Description page of Project Settings.


#include "TurtleWheel.h"

// Sets default values
ATurtleWheel::ATurtleWheel(){
	PrimaryActorTick.bCanEverTick = true;
	this->sWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtleWheel"));
	RootComponent = this->sWheelMesh;
}

// Called when the game starts or when spawned
void ATurtleWheel::BeginPlay(){
	Super::BeginPlay();
	this->sWheelMesh->SetMobility(EComponentMobility::Movable);
	this->sWheelMesh->SetSimulatePhysics(true);
}

// Called every frame
void ATurtleWheel::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

