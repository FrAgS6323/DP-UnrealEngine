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
    this->sMeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
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

    if (this->sMeshComp){
       //UE_LOG(LogTemp, Warning, TEXT("RotationVel: %f"), this->rotationVel);
       
       //UE_LOG(LogTemp, Warning, TEXT("RotationTorque: %f"), this->rotationTorque);

       //this->meshComp->SetRelativeRotation(FRotator(0.0f, 0.0f, this->angle)); //good but needs some tweaking!
       //this->meshComp->SetPhysicsAngularVelocityInDegrees(FVector(0.0, 0.0, this->rotationVel));
       
       //this->meshComp->WakeAllRigidBodies();
       this->sMeshComp->AddTorqueInRadians(FVector(this->rotationTorque, 0.0, 0.0), NAME_None, false);
       
#if 0
       // Get the current rotation of the component relative to its initial orientation
       FRotator CurrentRotation = this->meshComp->GetRelativeRotation();
       float CurrentAngle = CurrentRotation.Yaw; // Assuming rotation around the Yaw axis

       // Calculate the difference between the current angle and the target angle
       float AngleDifference = this->angle - CurrentAngle;

       // Define a threshold for stopping the rotation when close to the target
       const float RotationThreshold = 0.5f;  // Stop rotating when within 0.5 degrees

       if (FMath::Abs(AngleDifference) > RotationThreshold)
       {
           // Define a small torque value that simulates the servo motion
           constexpr float clampVal = 50000.0f;
           float TorqueStrength = FMath::Clamp(AngleDifference * 10000.0f, -clampVal, clampVal); // Adjust gain for servo-like effect
           FVector Torque = FVector(TorqueStrength, 0.0f, 0.0f);  // X-axis torque for Yaw
           UE_LOG(LogTemp, Warning, TEXT("Torque: %f AngleDiff: %f"), TorqueStrength, AngleDifference);
           // Apply the torque to the component
           this->meshComp->WakeAllRigidBodies();
           this->meshComp->AddTorqueInRadians(Torque, NAME_None, true);
       }
       else
       {
           // When within the threshold, stop applying torque
           this->meshComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
       }
#endif
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