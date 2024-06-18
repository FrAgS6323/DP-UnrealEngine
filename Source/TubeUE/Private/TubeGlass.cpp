// Fill out your copyright notice in the Description page of Project Settings.


#include "TubeGlass.h"

// Sets default values
ATubeGlass::ATubeGlass(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->tubeGlassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TubeGlassMesh"));
	RootComponent = this->tubeGlassMesh;
}

// Called when the game starts or when spawned
void ATubeGlass::BeginPlay(){
	Super::BeginPlay();
}

// Called every frame
void ATubeGlass::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

