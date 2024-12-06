#include "RotateWheelRight.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

URotateWheelRight::URotateWheelRight() {
	PrimaryComponentTick.bCanEverTick = true;
}

void URotateWheelRight::BeginPlay() {
	Super::BeginPlay();
	this->owner = GetOwner();
	this->sMeshComp = Cast<UStaticMeshComponent>(this->owner->GetRootComponent());
	this->rotationVel = URotateWheelRight::rotationVelForward;

	switch (this->eTurtleType) {
		case TurtleBotTypeEnum::ONE:
			this->ticks = this->ticksOne;
			break;
		case TurtleBotTypeEnum::TWO:
			this->ticks = this->ticksTwo;
			break;
		case TurtleBotTypeEnum::THREE:
			this->ticks = this->ticksThree;
			break;
	}
}

void URotateWheelRight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (this->sMeshComp) {
		this->ticksPassed++;
		FRotator rotation = this->owner->GetActorRotation();
		//UE_LOG(LogTemp, Warning, TEXT("Name: %s, Rotation.Yaw(z): %f"), *this->owner->GetName(), rotation.Yaw);
		//UE_LOG(LogTemp, Warning, TEXT("Name: %s, Rotation.Roll(x): %f, Rotation.Pitch(y): %f, Rotation.Yaw(z): %f, "), *this->owner->GetName(), rotation.Roll, rotation.Pitch, rotation.Yaw);

		//UE_LOG(LogTemp, Warning, TEXT("RotationVelocty: %f"), this->rotationVel);
		//UE_LOG(LogTemp, Warning, TEXT("Forward Vec: %s"), *this->owner->GetActorForwardVector().ToString());
		//FVector localAngularVelocity = this->owner->GetActorForwardVector() * this->rotationVel;
		
		//this->rotationVel = this->count > 82 ? 1.0 : this->rotationVel;

		UE_LOG(LogTemp, Warning, TEXT("TicksRight: %d, SetOfTicks: %d"), this->ticksPassed, this->ticks[this->idx]);

		if (this->ticksPassed > this->ticks[this->idx]) {
			this->rotationVel = URotateWheelRight::rotationVelStop;
			this->ticksPassed = 0;
			if (this->idx < this->ticks.Num() - 1) {
				this->idx++;
				this->actualSeriesOfTicks = this->ticks[this->idx];
			}
		}
		else {
			if ((this->actualSeriesOfTicks > 0 && this->ticks[this->idx] < 0) ||
				(this->actualSeriesOfTicks < 0 && this->ticks[this->idx] > 0))
				this->rotationVel = URotateWheelRight::rotationVelForward * (-1);
		}

		FVector localAngularVelocity = FVector(0.0, this->rotationVel, 0.0);
		//UE_LOG(LogTemp, Warning, TEXT("localAngularVelocity: %s"), *localAngularVelocity.ToString());
		FVector worldAngularVelocity = this->sMeshComp->GetComponentTransform().TransformVector(localAngularVelocity);
		//UE_LOG(LogTemp, Warning, TEXT("worldAngularVelocity: %s"), *worldAngularVelocity.ToString());
		
		this->sMeshComp->SetPhysicsAngularVelocityInDegrees(worldAngularVelocity, false);
		//this->sMeshComp->SetPhysicsAngularVelocityInDegrees(FVector(0.0, this->rotationVel, 0.0), false);

		//this->sMeshComp->AddTorqueInRadians(FVector(0.0, this->rotationTorque, 0.0), NAME_None, false);
	}
}