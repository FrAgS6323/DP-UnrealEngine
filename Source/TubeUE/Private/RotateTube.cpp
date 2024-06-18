// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateTube.h"

// Sets default values for this component's properties
URotateTube::URotateTube(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	this->isRotating = false;
	this->rotationSpeed = 100.0f; // Example rotation speed
}

void URotateTube::BeginPlay(){
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if (Owner){
		this->initQuat = Owner->GetActorQuat();
		this->currentQuat = this->initQuat;
	}
}

void URotateTube::rotate(float angleDeg){
	if (!this->isRotating){
		this->targetAngleDeg = angleDeg;
		this->currentAngleRad = 0.0f;
		this->isRotating = true;
        UE_LOG(LogTemp, Warning, TEXT("Starting Rotation: Target Angle = %f"), this->targetAngleDeg);
	}
}

void URotateTube::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (this->isRotating){
        float deltaAngle = this->rotationSpeed * DeltaTime;
        if (this->targetAngleDeg > 0){
            this->currentAngleRad += FMath::DegreesToRadians(deltaAngle);
            if (FMath::RadiansToDegrees(this->currentAngleRad) >= this->targetAngleDeg){
                this->isRotating = false;
                this->currentAngleRad = FMath::DegreesToRadians(this->targetAngleDeg);
            }
        }else{
            this->currentAngleRad -= FMath::DegreesToRadians(deltaAngle);
            if (FMath::RadiansToDegrees(this->currentAngleRad) <= this->targetAngleDeg){
                this->isRotating = false;
                this->currentAngleRad = FMath::DegreesToRadians(this->targetAngleDeg);
            }
        }

        FQuat DeltaQuat = FQuat(FRotator(0.0f, 0.0f, FMath::RadiansToDegrees(this->currentAngleRad)));
        this->currentQuat = this->initQuat * DeltaQuat;

        AActor* Owner = GetOwner();
        if (Owner) {
            Owner->SetActorRotation(this->currentQuat);
            UE_LOG(LogTemp, Warning, TEXT("Rotating: Current Angle = %f"), FMath::RadiansToDegrees(this->currentAngleRad));
        }
    }
}