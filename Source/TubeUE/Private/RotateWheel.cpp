#include "RotateWheel.h"

URotateWheel::URotateWheel(){
	PrimaryComponentTick.bCanEverTick = true;
}

void URotateWheel::BeginPlay(){
	Super::BeginPlay();
	this->sMeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
}

void URotateWheel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (this->sMeshComp) {
		UE_LOG(LogTemp, Warning, TEXT("RotationTorque: %f"), this->rotationTorque);
		//this->sMeshComp->SetPhysicsAngularVelocityInDegrees(FVector(0.0, this->rotationTorque, 0.0));
		this->sMeshComp->AddTorqueInRadians(FVector(0.0, this->rotationTorque, 0.0), NAME_None, false);
	}
}