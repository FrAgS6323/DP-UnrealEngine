// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

// Sets default values
ABall::ABall(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = this->ballMesh;

	//this->forceComponent = CreateDefaultSubobject<UForceComponent>(TEXT("ForceComponent"));
	this->ballMesh->SetSimulatePhysics(true);
	this->ballMesh->SetEnableGravity(true);
	this->ballMesh->SetMassOverrideInKg(NAME_None, 0.0027f);
	//this->ballMesh->GetBodyInstance()->MassScale = 0.0027f;
}

// Called when the game starts or when spawned
void ABall::BeginPlay(){
	Super::BeginPlay();
}

double ABall::getRadius(){
	return this->r;
}

// Called every frame
void ABall::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
#if 0
	UE_LOG(LogTemp, Warning, TEXT("BallPos x: %f"), this->GetActorLocation().X);
	UE_LOG(LogTemp, Warning, TEXT("BallPos y: %f"), this->GetActorLocation().Y);
	UE_LOG(LogTemp, Warning, TEXT("BallPos z: %f"), this->GetActorLocation().Z - this->r);
#endif
}