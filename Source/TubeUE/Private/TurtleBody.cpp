// Fill out your copyright notice in the Description page of Project Settings.


#include "TurtleBody.h"

// Sets default values
ATurtleBody::ATurtleBody(){
	this->sBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtleBody"));
	RootComponent = this->sBodyMesh;
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurtleBody::BeginPlay(){
	Super::BeginPlay();
	this->sBodyMesh->SetMobility(EComponentMobility::Movable);
	this->sBodyMesh->SetSimulatePhysics(true);
}

// Called every frame
void ATurtleBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}