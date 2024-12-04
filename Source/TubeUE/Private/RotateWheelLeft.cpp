#include "RotateWheelLeft.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

URotateWheelLeft::URotateWheelLeft() {
	PrimaryComponentTick.bCanEverTick = true;
}

void URotateWheelLeft::BeginPlay() {
	Super::BeginPlay();
	this->owner = GetOwner();
	this->sMeshComp = Cast<UStaticMeshComponent>(this->owner->GetRootComponent());
}

void URotateWheelLeft::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (this->sMeshComp) {
		this->count++;
		FRotator rotation = this->owner->GetActorRotation();
		UE_LOG(LogTemp, Warning, TEXT("Name: %s, Rotation.Yaw(z): %f"), *this->owner->GetName(), rotation.Yaw);
		//UE_LOG(LogTemp, Warning, TEXT("Name: %s, Rotation.Roll(x): %f, Rotation.Pitch(y): %f, Rotation.Yaw(z): %f, "), *this->owner->GetName(), rotation.Roll, rotation.Pitch, rotation.Yaw);

		//UE_LOG(LogTemp, Warning, TEXT("RotationVelocty: %f"), this->rotationVel);
		UE_LOG(LogTemp, Warning, TEXT("Forward Vec: %s"), *this->owner->GetActorForwardVector().ToString());
		//FVector localAngularVelocity = this->owner->GetActorForwardVector() * this->rotationVel;
		
		//this->rotationVel = this->count > 82 ? 1.0 : this->rotationVel;
		this->rotationVel = this->count > 40 ? 1.0 : this->rotationVel;

		FVector localAngularVelocity = FVector(0.0, this->rotationVel, 0.0);
		UE_LOG(LogTemp, Warning, TEXT("localAngularVelocity: %s"), *localAngularVelocity.ToString());
		FVector worldAngularVelocity = this->sMeshComp->GetComponentTransform().TransformVector(localAngularVelocity);
		UE_LOG(LogTemp, Warning, TEXT("worldAngularVelocity: %s"), *worldAngularVelocity.ToString());

		this->sMeshComp->SetPhysicsAngularVelocityInDegrees(worldAngularVelocity, false);
		//this->sMeshComp->SetPhysicsAngularVelocityInDegrees(FVector(0.0, this->rotationVel, 0.0), false);

		//this->sMeshComp->AddTorqueInRadians(FVector(0.0, this->rotationTorque, 0.0), NAME_None, false);
	}
}