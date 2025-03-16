#include "Tube.h"
#include <numbers>
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

    this->pidControllerBall = new UPID(ePIDusage::HEIGHT);
    //this->pidControllerBall->setSaturationLimits(this->saturationLimitBallMin, this->saturationLimitBallMax);

    this->pidControllerServo = new UPID(ePIDusage::ANGLE);
    //this->pidControllerServo->setSaturationLimits(this->saturationLimitServoMin, this->saturationLimitServoMax);

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
    this->sBallMesh->SetRelativeLocation(FVector(0, 0, -ATube::halfTubeHeight + (ballRadius * 4)));

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

//#if 0
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
//#endif
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
        actualAngle = this->sTubeMesh->GetComponentRotation().Roll - 90,
        raycastDistanceFromOriginX = 0,
        raycastDistanceFromOriginY = ATube::halfTubeHeight; //distanceFromTubeOriginToBottom;
    FVector raycastPos(0, 0, 0);

    if (0 != actualAngle) {
        if (actualAngle > 0) angleDegToSplit = 360 - actualAngle; 
        else angleDegToSplit = 180 + std::abs(actualAngle);

        //UE_LOG(LogTemp, Warning, TEXT("angleDegToSplit: %f"), angleDegToSplit);

        raycastDistanceFromOriginX = ATube::halfTubeHeight * cos(UEngineHelper::degToRad(angleDegToSplit));
        raycastDistanceFromOriginY = ATube::halfTubeHeight * sin(UEngineHelper::degToRad(angleDegToSplit));

        raycastPos.Y = -raycastDistanceFromOriginX;
        raycastPos.Z = -std::abs(raycastDistanceFromOriginY);
    }else{
        raycastPos.Z = -ATube::halfTubeHeight;
    }

    //UE_LOG(LogTemp, Warning, TEXT("raycastStartRelativeLoc-> Y(horiz): %f, Z(vert): %f"), raycastPos.Y, raycastPos.Z);
    
    this->bHit = UEngineHelper::performRaycast(this->GetWorld(),
                                               this->sTubeMesh,
                                               raycastPos,
                                               this->sTubeMesh->GetUpVector(),
                                               {this->sTubeMesh},
                                               true,
                                               ATube::halfTubeHeight * 2,
                                               this->distance);
}

auto ATube::splitBallForce(const double & PIDoutput, const double &inAngle) -> FVector {
    double beta = 180 - (std::abs(inAngle) + 90),
        xForce = 0,
        yForce = 0;
    FVector splitForce;
    //std::cout << "beta: " << beta << std::endl;

    if (0 == inAngle) {
        return FVector(0, 0, PIDoutput);
    }
    else {
        if (inAngle > 0) {
            xForce = abs(PIDoutput * cos(beta + 90)) * (-1);
            yForce = abs(PIDoutput * sin(beta + 90));
        }
        else if (inAngle < 0) {
            xForce = abs(PIDoutput * cos(beta));
            yForce = abs(PIDoutput * sin(beta));
        }
        else {
            xForce = 0;
            yForce = PIDoutput;
        }
        splitForce.Y = xForce;
        splitForce.Z = yForce;
    }

    //UE_LOG(LogTemp, Warning, TEXT("splitForce-> Y(horiz): %f, Z(vert): %f"), splitForce.Y, splitForce.Z);
    return splitForce;
}

void ATube::PIDBall(float deltaTime) {
//#if 0
    if ((this->desiredHeight > this->getRegulationHeight()) ||
        (this->desiredHeight < 0))
        UE_LOG(LogTemp, Warning, TEXT("Wrong input! (Expected between 0 and %f)"), this->getRegulationHeight());
//#endif
    if (!this->pidControllerBall) UE_LOG(LogTemp, Warning, TEXT("UPID for ball not initialized!"));

    if (!this->bPidBallSwitch) return;

    double error = UPID::estimateError(this->desiredHeight, this->distance),
           //output = this->pidController->getPIDOutput(error, (double)deltaTime);
           output = this->pidControllerBall ? this->pidControllerBall->getPIDOutput(error, (double)deltaTime) : 0;
    
    UE_LOG(LogTemp, Warning, TEXT("PID Ball error: = %f"), error);
    UE_LOG(LogTemp, Warning, TEXT("PID Ball output: = %f"), output);
    if (this->sBallMesh && this->bHit) this->sBallMesh->AddForce(this->splitBallForce(output, this->sTubeMesh->GetComponentRotation().Roll));
}

void ATube::PIDServo(float deltaTime) {
    if (std::abs(this->angle) > this->maxAngle) UE_LOG(LogTemp, Warning, TEXT("Wrong angle! (Expected between -%f and %f)"), this->maxAngle, this->maxAngle);

    if (!this->pidControllerServo) UE_LOG(LogTemp, Warning, TEXT("UPID for servo not initialized!"));

    FRotator currentRotation = this->sTubeMesh->GetComponentRotation();
    double error = UPID::estimateError(this->angle, currentRotation.Roll),
        output = this->pidControllerServo ? this->pidControllerServo->getPIDOutput(error, (double)deltaTime) : 0;

    //UE_LOG(LogTemp, Warning, TEXT("PID Servo error: = %f"), error);
    //UE_LOG(LogTemp, Warning, TEXT("PID Servo output: = %f"), output);
    if (this->sTubeMesh) this->sTubeMesh->AddTorqueInRadians(FVector(output, 0.0, 0.0), NAME_None, false);
}

void ATube::rotate(double inAngleDeg){
    if (std::abs(inAngleDeg) > this->maxAngle) std::runtime_error("Wrong angle!");

    FRotator currentRotation = this->sTubeMesh->GetComponentRotation();
    double roundCurrRotation = round(currentRotation.Roll),
           deltaAngleDeg = inAngleDeg - roundCurrRotation;

    if (inAngleDeg != this->angle) {
        if (deltaAngleDeg < 0 && roundCurrRotation != inAngleDeg) this->angle -= this->numRotationVel;
        else if (deltaAngleDeg > 0 && roundCurrRotation != inAngleDeg) this->angle += this->numRotationVel;
    }
    //UE_LOG(LogTemp, Warning, TEXT("AngleDeg: %f currRotRoll: %f  roundCurrRotation: %f delta: %f"), this->angle, currentRotation.Roll, roundCurrRotation, deltaAngleDeg);

    FRotator newRotation = FRotator(currentRotation.Pitch,
                                    currentRotation.Yaw,
                                    45.0);

    this->sTubeMesh->SetWorldTransform(FTransform(newRotation, this->sTubeMesh->GetComponentLocation()), false, nullptr, ETeleportType::None);
    
    //this->sTubeMesh->SetPhysicsAngularVelocityInDegrees(FVector(angle, 0.0f, 0.0f), false);
}

void ATube::convertXYToHeightAngle(double x,
                                   double y,
                                   double& outHeight,
                                   double& outAngle) {
    double d,
        s,
        c = std::sqrt((x * x) +
            (y * y));

    if (y < 0) c *= (-1);
    d = 20 + c;

    if (c != 0) {
        s = UEngineHelper::radToDeg(std::asin(x / c));
    }
    else {
        s = 0;
    }

    d = std::round(d);

    if (d >= 10 && d <= 40) s = round(s);
    if (s < 0 && s > -10) s *= (-1);

    outHeight = d;
    outAngle = s;

    outAngle *= (-1);
}

void ATube::convertHeightAngleToXY(double inHeight,
                                   double inAngle,
                                   double& xOut,
                                   double& yOut) {
    double a,
        b,
        c,
        x,
        y,
        ym = 1,
        xm = -1;
    //if(height != 0){
    if (inAngle < 0) {
        inAngle *= (-1);
        xm = 1;
    }
    inAngle = (std::numbers::pi / 180) * (inAngle);

    c = static_cast<double>(inHeight) - 20;
    if (c < 0) {
        c *= (-1);
        ym = (-1);
    }
    a = std::sin(inAngle) * c;
    b = std::sqrt((c * c) - (a * a));
    x = (a * xm * ym * 10) / 10;// /10 / 100
    y = (b * ym * 10) / 10;
    if (0 == inHeight) {
        //x = 0;
        y = -20; // -20
    }
    xOut = x;
    yOut = y;
}

void ATube::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

    this->SetActorTickInterval(-10.0f);
    
    this->bSetIdealBallPID
        ? 
        this->pidControllerBall->setIdealPIDvalues() 
        : 
        this->pidControllerBall->setPIDvalues(this->ballP,
                                              this->ballI,
                                              this->ballD);

    this->bSetIdealServoPID
        ?
        this->pidControllerServo->setIdealPIDvalues()
        :
        this->pidControllerServo->setPIDvalues(this->servoP,
                                               this->servoI,
                                               this->servoD);
    this->pidControllerBall->setSaturationLimits(this->saturationLimitBallMin, this->saturationLimitBallMax);
    this->pidControllerServo->setSaturationLimits(this->saturationLimitServoMin, this->saturationLimitServoMax);

    this->performRaycast();

    if (!this->bCoords) {
        this->desiredHeight = this->setDesiredHeight;
        this->angle = this->setAngle;        
    }else{
        this->bPidBallSwitch = true;
        this->convertXYToHeightAngle(this->xBallPos, 
                                     this->yBallPos, 
                                     this->desiredHeight,
                                     this->angle);

        //UE_LOG(LogTemp, Warning, TEXT("Height: %f Angle: %f"), this->desiredHeight, this->angle);
    }

    this->PIDBall(DeltaTime);
    this->PIDServo(DeltaTime);
    //this->rotate(this->inAngle);
}

#if 0
ATube::~ATube(){
    if (this->pidController) delete this->pidController;
}
#endif