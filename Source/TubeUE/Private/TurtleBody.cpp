#include "TurtleBody.h"
#include "../EngineHelper.h"

// Sets default values
ATurtleBody::ATurtleBody() {
    PrimaryActorTick.bCanEverTick = true;
    this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponentTurtlebot"));
    this->sBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotBody"));
    this->sLeftWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotLeftWheel"));
    this->sRightWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotRightWheel"));
    this->sLeftBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotLeftBall"));
    this->sRightBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotRightBall"));
    this->sLidarBottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotLidarBottom"));
    this->sLidarTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotLidarTop"));
    this->leftWheelJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TurtlebotLeftWheelJoint"));
    this->rightWheelJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TurtlebotRightWheelJoint"));
    this->leftBallJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TurtlebotLeftBallJoint"));
    this->rightBallJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TurtlebotRightBallJoint"));
    this->lidarBottomJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TurtlebotLidarBottomJoint"));
    this->lidarTopJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TurtlebotLidarTopJoint"));
    
    if (this->RootComponent){
        if (this->sBodyMesh) this->sBodyMesh->SetupAttachment(this->RootComponent);
        if (this->sLeftWheelMesh) this->sLeftWheelMesh->SetupAttachment(this->sBodyMesh);
        if (this->sRightWheelMesh) this->sRightWheelMesh->SetupAttachment(this->sBodyMesh);
        if (this->sLeftBallMesh) this->sLeftBallMesh->SetupAttachment(this->sBodyMesh);
        if (this->sRightBallMesh) this->sRightBallMesh->SetupAttachment(this->sBodyMesh);
        if (this->sLidarBottomMesh) this->sLidarBottomMesh->SetupAttachment(this->sBodyMesh);
        if (this->sLidarTopMesh) this->sLidarTopMesh->SetupAttachment(this->sBodyMesh);
    }else{
        UE_LOG(LogTemp, Error, TEXT("RootComponentTurtlebot not initialized!"));
    }

    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/TurtleBot/TurtlebotBody"), this->sBodyMesh);
    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/TurtleBot/TurtlebotWheelLeft"), this->sLeftWheelMesh);
    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/TurtleBot/TurtlebotWheelRight"), this->sRightWheelMesh);
    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/TurtleBot/TurtlebotBall"), this->sLeftBallMesh);
    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/TurtleBot/TurtlebotBall"), this->sRightBallMesh);
    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/TurtleBot/TurtlebotLidarBottom"), this->sLidarBottomMesh);
    UEngineHelper::loadMeshDynamic(TEXT("/Game/Models/TurtleBot/TurtlebotLidarTop"), this->sLidarTopMesh);

    this->initialize();
}

void ATurtleBody::initialize(){
    //this->sLeftWheelMesh->SetWorldLocation(this->GetActorLocation() + FVector(6.25, -15, 0));
    this->sLeftWheelMesh->SetRelativeLocation(FVector(6.25, -15.5, 0));
    this->leftWheelJoint->SetRelativeLocation(this->sLeftWheelMesh->GetRelativeLocation());
    this->sRightWheelMesh->SetRelativeLocation(FVector(6.25, 15.5, 0));
    this->rightWheelJoint->SetRelativeLocation(this->sRightWheelMesh->GetRelativeLocation());
    this->sLeftBallMesh->SetRelativeLocation(FVector(-11.25, -6.5, -2));
    this->leftBallJoint->SetRelativeLocation(this->sLeftBallMesh->GetRelativeLocation());
    this->sRightBallMesh->SetRelativeLocation(FVector(-11.25, 6.5, -2));
    this->rightBallJoint->SetRelativeLocation(this->sRightBallMesh->GetRelativeLocation());
    this->sLidarBottomMesh->SetRelativeLocation(FVector(0, 0, 11));
    this->lidarBottomJoint->SetRelativeLocation(this->sLidarBottomMesh->GetRelativeLocation());
    this->sLidarTopMesh->SetRelativeLocation(FVector(0, 0, 11.25));
    this->lidarTopJoint->SetRelativeLocation(this->sLidarTopMesh->GetRelativeLocation());

    this->sBodyMesh->SetSimulatePhysics(true);
    this->sLeftWheelMesh->SetSimulatePhysics(true);
    this->sRightWheelMesh->SetSimulatePhysics(true);
    this->sLeftBallMesh->SetSimulatePhysics(true);
    this->sRightBallMesh->SetSimulatePhysics(true);
    this->sLidarBottomMesh->SetSimulatePhysics(true);
    this->sLidarTopMesh->SetSimulatePhysics(true);

    this->sBodyMesh->SetEnableGravity(true);
    this->sLeftWheelMesh->SetEnableGravity(true);
    this->sRightWheelMesh->SetEnableGravity(true);
    this->sLeftBallMesh->SetEnableGravity(true);
    this->sRightBallMesh->SetEnableGravity(true);
    this->sLidarBottomMesh->SetEnableGravity(true);
    this->sLidarTopMesh->SetEnableGravity(true);

    if (this->sBodyMesh) {
        this->sBodyMesh->SetMobility(EComponentMobility::Movable);
        this->sBodyMesh->SetCollisionProfileName("PhysicsActor");
        this->sBodyMesh->SetWorldLocation(this->GetActorLocation());
        this->sBodyMesh->SetMassOverrideInKg(NAME_None, 1.0f, true);

        UEngineHelper::setupConstraint(this->leftWheelJoint,
            this->sBodyMesh,
            this->sBodyMesh,
            this->sLeftWheelMesh,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f);

        UEngineHelper::setupConstraint(this->rightWheelJoint,
            this->sBodyMesh,
            this->sBodyMesh,
            this->sRightWheelMesh,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f);

        UEngineHelper::setupConstraint(this->leftBallJoint,
            this->sBodyMesh,
            this->sBodyMesh,
            this->sLeftBallMesh,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f);

        UEngineHelper::setupConstraint(this->rightBallJoint,
            this->sBodyMesh,
            this->sBodyMesh,
            this->sRightBallMesh,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f);
//#if 0

        UEngineHelper::setupConstraint(this->lidarBottomJoint,
            this->sBodyMesh,
            this->sBodyMesh,
            this->sLidarBottomMesh,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f);

        UEngineHelper::setupConstraint(this->lidarTopJoint,
            this->sBodyMesh,
            this->sLidarBottomMesh,
            this->sLidarTopMesh,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            ELinearConstraintMotion::LCM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Free, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f,
            EAngularConstraintMotion::ACM_Locked, 0.0f);
//#endif
//#if 0
        this->sLeftWheelMesh->SetMobility(EComponentMobility::Movable);
        this->sLeftWheelMesh->SetMassOverrideInKg(NAME_None, 0.2f, true);
        this->sLeftWheelMesh->BodyInstance.bAutoWeld = false;

        this->sRightWheelMesh->SetMobility(EComponentMobility::Movable);
        this->sRightWheelMesh->SetMassOverrideInKg(NAME_None, 0.2f, true);
        this->sRightWheelMesh->BodyInstance.bAutoWeld = false;

        this->sLeftBallMesh->SetMobility(EComponentMobility::Movable);
        this->sLeftBallMesh->SetMassOverrideInKg(NAME_None, 0.1f, true);
        this->sLeftBallMesh->BodyInstance.bAutoWeld = false;

        this->sRightBallMesh->SetMobility(EComponentMobility::Movable);
        this->sRightBallMesh->SetMassOverrideInKg(NAME_None, 0.1f, true);
        this->sRightBallMesh->BodyInstance.bAutoWeld = false;
//#endif
        this->sLidarBottomMesh->SetMobility(EComponentMobility::Movable);
        this->sLidarBottomMesh->SetMassOverrideInKg(NAME_None, 0.2f, true);
        this->sLidarBottomMesh->BodyInstance.bAutoWeld = false;

        this->sLidarTopMesh->SetMobility(EComponentMobility::Movable);
        this->sLidarTopMesh->SetMassOverrideInKg(NAME_None, 0.35f, true);
        this->sLidarTopMesh->BodyInstance.bAutoWeld = false;
    }
}
#if 0
void ATurtleBody::OnConstruction(const FTransform& Transform){
    Super::OnConstruction(Transform);
    this->initialize();
}
#endif

// Called when the game starts or when spawned
void ATurtleBody::BeginPlay(){
    Super::BeginPlay();
    //UE_LOG(LogTemp, Warning, TEXT("Begin Play called!"));
}

void ATurtleBody::rotateWheel(UStaticMeshComponent* wheel, double direction){
    FVector localAngularVelocity = FVector(0.0f, ATurtleBody::wheelRotationVel * direction, 0.0f);
    FVector worldAngularVelocity = wheel->GetComponentTransform().TransformVector(localAngularVelocity);

    wheel->SetPhysicsAngularVelocityInDegrees(worldAngularVelocity, false);
}

void ATurtleBody::moveFB(float value){
    if (FMath::Abs(value) > 0.1f) {
        this->rotateWheel(this->sLeftWheelMesh, value);
        this->rotateWheel(this->sRightWheelMesh, value);
    }
}

void ATurtleBody::moveLR(float value){
    if (FMath::Abs(value) > 0.1f) {
        this->rotateWheel(this->sLeftWheelMesh, value);
        this->rotateWheel(this->sRightWheelMesh, -value);
    }
}

void ATurtleBody::moveRobot(float value){
    if (this->bForward) this->moveFB(value);
    else if (this->bBackward) this->moveFB(-value);
    else if (this->bTurnLeft) this->moveLR(-value);
    else if (this->bTurnRight) this->moveLR(value);
}

void ATurtleBody::turnLidar(double direction, bool bRaycast){
    FVector localAngularVelocity = FVector(0.0f, 0.0f, ATurtleBody::lidarRotationVel * direction);
    FVector worldAngularVelocity = this->sLidarTopMesh->GetComponentTransform().TransformVector(localAngularVelocity);
    double rayLength = 250.0f, 
           hitDistance = 0.0f;

    if (bRaycast){
        UEngineHelper::performRaycast(this->GetWorld(),
                                      this->sLidarTopMesh,
                                      FVector(0, 0, 0),
                                      this->sLidarTopMesh->GetForwardVector(),
                                      { this->sLidarTopMesh, this->sLidarBottomMesh },
                                      bRaycast,
                                      rayLength,
                                      hitDistance);
            //if(hitDistance > 0) UE_LOG(LogTemp, Warning, TEXT("Lidar hit distance: %ld"), hitDistance);
    }
    this->sLidarTopMesh->SetPhysicsAngularVelocityInDegrees(worldAngularVelocity, false);
}

void ATurtleBody::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("MoveFB", this, &ATurtleBody::moveFB);
    PlayerInputComponent->BindAxis("MoveLR", this, &ATurtleBody::moveLR);
}

// Called every frame
void ATurtleBody::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
    this->moveRobot(0.5);

    //this->rotateWheel(this->sLeftWheelMesh, 0.5f);
    //this->rotateWheel(this->sRightWheelMesh, -0.5f);
    this->bTurnLidar ? this->turnLidar(0.5f, true) : this->turnLidar(0.0f, false);
}