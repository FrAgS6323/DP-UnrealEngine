#include "RotateTube.h"

// Sets default values for this component's properties
URotateTube::URotateTube(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//this->isRotating = false;
	//this->rotationSpeed = 100.0f; // Example rotation speed
}

void URotateTube::BeginPlay(){
	Super::BeginPlay();
    this->meshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
#if 0
	AActor* Owner = GetOwner();
	if (Owner){
		this->initQuat = Owner->GetActorQuat();
		this->currentQuat = this->initQuat;
	}
#endif
}

#if 0
void URotateTube::rotate(float angleDeg){
	if (!this->isRotating){
		this->targetAngleDeg = angleDeg;
		this->currentAngleRad = 0.0f;
		this->isRotating = true;
        UE_LOG(LogTemp, Warning, TEXT("Starting Rotation: Target Angle = %f"), this->targetAngleDeg);
	}
}
#endif

void URotateTube::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (this->meshComp){
        //UE_LOG(LogTemp, Warning, TEXT("RotationVelDeg: %f"), this->rotationVel);
        //this->meshComp->SetRelativeRotation(FRotator(0.0f, 0.0f, this->angle));
        this->meshComp->SetPhysicsAngularVelocityInDegrees(FVector(0.0, 0.0, this->rotationVel));
    }

#if 0
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
#endif
}