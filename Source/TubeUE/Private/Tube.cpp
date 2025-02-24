#include "Tube.h"
#include "../EngineHelper.h"
#include "PID.h"

ATube::ATube(){
	PrimaryActorTick.bCanEverTick = true;
    this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponentTube"));
	this->sTubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MergedTubeMesh"));
    this->sHolderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HolderMesh"));
    this->sBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    this->tubeJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TubeJoint"));

    if (this->RootComponent){
        this->sHolderMesh->SetupAttachment(this->RootComponent);
        this->sTubeMesh->SetupAttachment(this->sHolderMesh);
        this->sBallMesh->SetupAttachment(this->sTubeMesh);
    }else{
        UE_LOG(LogTemp, Error, TEXT("RootComponentTube not initialized!"));
    }

    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/Tube/MergedTubeMesh"), this->sTubeMesh);
    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/Holder/Holder"), this->sHolderMesh);
    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/Ball/Ball"), this->sBallMesh);

    this->pidController = new UPID();
    this->pidController->setSaturationLimits(0.0, 10.0);

    this->initialize();
}

void ATube::BeginPlay(){
	Super::BeginPlay();
#if 0
	FVector meshBounds = this->sTubeMesh->Bounds.BoxExtent;
	FVector centerOfMassOffset = FVector(0.0f, 0.0f, -meshBounds.Z);

	this->sTubeMesh->SetCenterOfMass(centerOfMassOffset, NAME_None);
#endif
}

void ATube::initialize(){
    constexpr double ballRadius = 0.015;
    this->sTubeMesh->SetRelativeLocation(FVector(6.0, 0, 0));
    this->sBallMesh->SetRelativeLocation(FVector(0, 0, -ATube::halfTubeHeight + ballRadius));
    //this->leftWheelJoint->SetRelativeLocation(this->sLeftWheelMesh->GetRelativeLocation());

    this->sHolderMesh->SetSimulatePhysics(true);
    this->sTubeMesh->SetSimulatePhysics(true);
    this->sBallMesh->SetSimulatePhysics(true);

    this->sHolderMesh->SetEnableGravity(true);
    this->sTubeMesh->SetEnableGravity(true);
    this->sBallMesh->SetEnableGravity(true);

    if (this->sHolderMesh){
        this->sHolderMesh->SetWorldLocation(this->GetActorLocation());
        this->sHolderMesh->SetRelativeRotation(FRotator(0, 180, 0));
        this->sHolderMesh->SetMobility(EComponentMobility::Movable);
        this->sHolderMesh->SetWorldLocation(this->GetActorLocation());
        this->sHolderMesh->SetMassOverrideInKg(NAME_None, 10.0f, true);

        this->sTubeMesh->SetMobility(EComponentMobility::Movable);

        UEngineHelper::setupConstraint(this->tubeJoint,
            this->sHolderMesh,
            this->sHolderMesh,
            this->sTubeMesh,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f);

        //this->tubeJoint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
        //this->tubeJoint->SetAngularOrientationDrive(false , true);
        //this->tubeJoint->SetAngularDriveParams(1000000.0f, 5000.0f, 1000000.0f); // Sila a tlmenie

        //this->sTubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

        this->sTubeMesh->SetMobility(EComponentMobility::Movable);
        this->sTubeMesh->SetMassOverrideInKg(NAME_None, 0.15f, true);
        this->sTubeMesh->BodyInstance.bAutoWeld = false;

        this->sBallMesh->SetMobility(EComponentMobility::Movable);
        this->sBallMesh->SetMassOverrideInKg(NAME_None, 0.0027f, true);
        this->sBallMesh->BodyInstance.bAutoWeld = false;
    }
}

auto ATube::getDistance() -> double {
	return (this->distance > 0.001) ? this->distance : 0;
}

auto ATube::getRegulationHeight() -> double {
    return 45.0f;
}

void ATube::performRaycast() {
        //distanceFromTubeOriginToBottom = (this->height - this->motorHeight - this->upperHeight) / 2,
    double angleDegToSplit = 0,
        raycastDistanceFromOriginY = 0,
        raycastDistanceFromOriginZ = ATube::halfTubeHeight; //distanceFromTubeOriginToBottom;
    FVector tubePos = this->sTubeMesh->GetComponentLocation(),
            raycastPos(tubePos.X, tubePos.Y, tubePos.Z);

    if (0 != this->angleDeg) {
        if (this->angleDeg > 0) angleDegToSplit = 360 - this->angleDeg;
        else angleDegToSplit = 180 + abs(this->angleDeg);

        raycastDistanceFromOriginY = ATube::halfTubeHeight * cos(UEngineHelper::degToRad(angleDegToSplit));
        raycastDistanceFromOriginZ = ATube::halfTubeHeight * sin(UEngineHelper::degToRad(angleDegToSplit));

        raycastPos.Y = tubePos.Y + raycastDistanceFromOriginY;

        raycastPos.Z = tubePos.Z - abs(raycastDistanceFromOriginZ);
    }
    else{
        raycastPos.Z = tubePos.Z - raycastDistanceFromOriginZ;
    }

#if 0
    UEngineHelper::performRaycast(this,
        FVector(-tubePos.X,
            tubePos.Y,
            tubePos.Z),
        this->sTubeMesh->GetUpVector(),
        meshesToExclude,
        true,
        rayLength,
        this->distance);

#endif
    //FVector tubePos = this->sTubeMesh->GetRelativeLocation(),
#if 0
        startVec = FVector(tubePos.X,
            tubePos.Y,
            tubePos.Z - ATube::halfTubeHeight),
#endif
    FVector endVec = ((this->sTubeMesh->GetUpVector() * 50.0f) + raycastPos);

    FHitResult hitResult;
    FCollisionQueryParams collisionParams;
    collisionParams.AddIgnoredComponent(this->sTubeMesh);

    bool bIsHit = this->GetWorld()->LineTraceSingleByChannel(hitResult,
        raycastPos,
        endVec,
        ECC_Visibility,
        collisionParams);

    if (bIsHit) {
        //UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitResult.GetActor()->GetName());

        this->distance = hitResult.Distance;
        //UE_LOG(LogTemp, Warning, TEXT("Hit Distance: %f"), this->distance);
        //DrawDebugLine(GetWorld(), startVec, endVec, FColor::Red, false, 1, 0, 1);
        // 
        
        DrawDebugLine(GetWorld(), raycastPos, endVec, FColor::Green, false, 1, 0, 1);
        //DrawDebugPoint(GetWorld(), hitResult.Location, 10, FColor::Green, false, 1);
    }
    else {
        DrawDebugLine(GetWorld(), raycastPos, endVec, FColor::Red, false, 1, 0, 1);
    }
}

void ATube::PIDreg(float deltaTime) {
    if ((this->desiredHeight > this->getRegulationHeight()) ||
        (this->desiredHeight < 0))
        UE_LOG(LogTemp, Warning, TEXT("Wrong input! (Expected between 0 and %f)"), this->getRegulationHeight());

    if (!this->bPidSwitch) return;

    if (!this->pidController) UE_LOG(LogTemp, Warning, TEXT("UPID not initialized!"));

    double error = UPID::estimateError(this->desiredHeight, this->distance),
           //output = this->pidController->getPIDOutput(error, (double)deltaTime);
           output = this->pidController ? this->pidController->getPIDOutput(error, (double)deltaTime) : 0;
    
    UE_LOG(LogTemp, Warning, TEXT("PIDerror: = %f"), error);
    UE_LOG(LogTemp, Warning, TEXT("PIDoutput: = %f"), output);
    if (this->sBallMesh) this->sBallMesh->AddForce(FVector(0, 0, output));
}

void ATube::rotate(double inAngleDeg){
    if (std::abs(inAngleDeg) > this->maxAngle) std::runtime_error("Wrong angle!");

    FRotator currentRotation = this->sTubeMesh->GetComponentRotation();
    double roundCurrRotation = round(currentRotation.Roll),
           deltaAngleDeg = inAngleDeg - roundCurrRotation;

    if (deltaAngleDeg < 0 && roundCurrRotation != inAngleDeg) this->angleDeg -= this->numRotationVel;
    else if (deltaAngleDeg > 0 && roundCurrRotation != inAngleDeg) this->angleDeg += this->numRotationVel;

    UE_LOG(LogTemp, Warning, TEXT("AngleDeg: %f currRotRoll: %f  roundCurrRotation: %f delta: %f"), this->angleDeg, currentRotation.Roll, roundCurrRotation, deltaAngleDeg);

    FRotator newRotation = FRotator(currentRotation.Pitch,
                                    currentRotation.Yaw,
                                    this->angleDeg);

    this->sTubeMesh->SetWorldTransform(FTransform(newRotation, this->sTubeMesh->GetComponentLocation()), false, nullptr, ETeleportType::None);
    
    //this->sTubeMesh->SetPhysicsAngularVelocityInDegrees(FVector(angleDeg, 0.0f, 0.0f), false);
}

void ATube::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

    //UE_LOG(LogTemp, Warning, TEXT("Tube absolute location: %s"), this->sTubeMesh->IsUsingAbsoluteLocation() ? TEXT("True") : TEXT("False"));
    constexpr double rayLength = 50.0;
    FVector tubePos = this->sTubeMesh->GetRelativeLocation();
    TArray<UPrimitiveComponent*> meshesToExclude = {this->sTubeMesh};
#if 0
    UEngineHelper::performRaycast(this, 
                                  FVector(-tubePos.X,
                                          tubePos.Y,
                                          tubePos.Z), 
                                  this->sTubeMesh->GetUpVector(),
                                  meshesToExclude,
                                  true, 
                                  rayLength,
                                  this->distance);
#endif

    //if (this->distance > 0) UE_LOG(LogTemp, Warning, TEXT("Tube hit distance: %ld"), this->distance);
    
    this->bSetIdealPID 
        ? 
        this->pidController->setIdealPIDvalues() 
        : 
        this->pidController->setPIDvalues(this->P,
                                          this->I,
                                          this->D);
    this->PIDreg(DeltaTime);
    this->rotate(this->inAngle);
    this->performRaycast();
}

#if 0
ATube::~ATube(){
    if (this->pidController) delete this->pidController;
}
#endif