#include "Tube.h"
#include "../EngineHelper.h"
#include "PID.h"
#include "Json.h"
#include <numbers>
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"
#include "Trace/Trace.h"

ATube::ATube(){
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 0.013f;

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
    this->pidControllerServo = new UPID(ePIDusage::ANGLE);

    this->initialize();

#if 0
    TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)> functor = [this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
        this->funcForWebHandler(request, response, connected);
        };
#endif

    this->webHandler = new WebHandler(TEXT("http://147.232.60.231:5001/tubeDataAPI"), WebHandler::eRequestType::GET);
}

void ATube::BeginPlay(){
	Super::BeginPlay();
    
    if (!this->widgetComponent) {
        TArray<UWidgetComponent*> WidgetComponents;
        GetComponents<UWidgetComponent>(WidgetComponents);

        for (UWidgetComponent* WidgetComp : WidgetComponents)
        {
            if (WidgetComp->GetName() == TEXT("WidgetForTube"))
            {
                this->widgetComponent = WidgetComp;
                UUserWidget* homeWidget = Cast<UUserWidget>(widgetComponent->GetUserWidgetObject());

                if (homeWidget){
                    this->widgetSwitcher = Cast<UWidgetSwitcher>(homeWidget->GetWidgetFromName(TEXT("WidgetSwitcher")));

                    UButton* buttonSim = Cast<UButton>(homeWidget->GetWidgetFromName(TEXT("SimulationButton")));
                    buttonSim->OnClicked.AddDynamic(this, &ATube::bindOnButtonSimClick);

                    UButton* buttonViz = Cast<UButton>(homeWidget->GetWidgetFromName(TEXT("VisualizationButton")));
                    buttonViz->OnClicked.AddDynamic(this, &ATube::bindOnButtonVizClick);

                    UButton* buttonSimIRL = Cast<UButton>(homeWidget->GetWidgetFromName(TEXT("SimInRealTimeButton")));
                    buttonSimIRL->OnClicked.AddDynamic(this, &ATube::bindOnButtonSimIRLClick);

                    UCheckBox* pidCheckBox = Cast<UCheckBox>(homeWidget->GetWidgetFromName(TEXT("bPID")));
                    pidCheckBox->OnCheckStateChanged.AddDynamic(this, &ATube::bindOnRegCheckboxChange);

                    UCheckBox* idealPIDCheckBox = Cast<UCheckBox>(homeWidget->GetWidgetFromName(TEXT("bIdealPID")));
                    idealPIDCheckBox->OnCheckStateChanged.AddDynamic(this, &ATube::bindOnidealPIDCheckboxChange);

                    UCheckBox* bCoordCheckBox = Cast<UCheckBox>(homeWidget->GetWidgetFromName(TEXT("bCoord")));
                    bCoordCheckBox->OnCheckStateChanged.AddDynamic(this, &ATube::bindOnCoordsCheckboxChange);

                    UEditableTextBox* editableBoxHeight = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("HeightInput")));
                    editableBoxHeight->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitHeight);

                    UEditableTextBox* editableBoxAngle = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("AngleInput")));
                    editableBoxAngle->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitAngle);

                    UEditableTextBox* editableBoxXInput = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("XInput")));
                    editableBoxXInput->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitX);

                    UEditableTextBox* editableBoxYInput = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("YInput")));
                    editableBoxYInput->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitY);

                    UEditableTextBox* editableBoxBallP= Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("ballPInput")));
                    editableBoxBallP->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitP);

                    UEditableTextBox* editableBoxBallI = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("ballIInput")));
                    editableBoxBallI->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitI);

                    UEditableTextBox* editableBoxBallD = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("ballDInput")));
                    editableBoxBallD->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitD);
                }
                break;
            }
        }
        if (!this->widgetComponent) {
            UE_LOG(LogTemp, Warning, TEXT("WidgetTube not initialized!"));
            return;
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("WidgetTube correctly initialized!"));
        }
    }
}

void ATube::funcForWebHandler(FHttpRequestPtr request, FHttpResponsePtr response, bool connected){
    if (response.IsValid()) {
        TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(response->GetContentAsString());
        FJsonSerializer::Deserialize(jsonReader, this->responseObj);

        FString value;
        if (this->responseObj->TryGetStringField("target", value)) LexTryParseString(this->desiredHeight, *value);
        if (this->responseObj->TryGetStringField("height", value)) LexTryParseString(this->vizActualHeight, *value);
        if (this->responseObj->TryGetStringField("angle", value)) LexTryParseString(this->angle, *value);

        //UE_LOG(LogTemp, Warning, TEXT("parsed-> target: %s"), *this->responseObj->GetStringField("target"));
    }
}

void ATube::initialize(){
    constexpr double ballRadius = 1.5;
    this->sTubeMesh->SetRelativeLocation(FVector(6.0, 0, 0));
    this->sBallMesh->SetRelativeLocation(FVector(0, 0, -ATube::halfTubeHeight + ballRadius));

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
        //this->tubeJoint->SetWorldLocation(this->GetActorLocation());
        //this->tubeJoint->SetRelativeRotation(FRotator(0,180,0));

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
        this->sTubeMesh->SetMobility(EComponentMobility::Movable);
        this->sTubeMesh->SetMassOverrideInKg(NAME_None, 0.15f, true);
        this->sTubeMesh->BodyInstance.bAutoWeld = false;

        this->sBallMesh->SetMobility(EComponentMobility::Movable);
        this->sBallMesh->SetMassOverrideInKg(NAME_None, 0.0027f, true);
        this->sBallMesh->BodyInstance.bAutoWeld = false;
    }
}

void ATube::setMeshesPhysicsAndGravity(bool bState){
    if (bState && !this->bPhysicsIsSet){
        this->sHolderMesh->SetSimulatePhysics(bState);
        this->sTubeMesh->SetSimulatePhysics(bState);
        this->sBallMesh->SetSimulatePhysics(bState);

        this->sHolderMesh->WakeAllRigidBodies();
        this->sTubeMesh->WakeAllRigidBodies();
        this->sBallMesh->WakeAllRigidBodies();

        this->sHolderMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        this->sTubeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        this->sBallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);

        this->sHolderMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
        this->sTubeMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
        this->sBallMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

        this->sHolderMesh->SetEnableGravity(bState);
        this->sTubeMesh->SetEnableGravity(bState);
        this->sBallMesh->SetEnableGravity(bState);

        this->bPhysicsIsSet = true;
    }else{
        FTransform savedHolderTransform = this->sHolderMesh->GetComponentTransform(),
            savedTubeTransform = this->sTubeMesh->GetComponentTransform(),
            savedBallTransform = this->sBallMesh->GetComponentTransform();

        this->sHolderMesh->SetWorldTransform(savedHolderTransform, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
        this->sTubeMesh->SetWorldTransform(savedTubeTransform, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
        this->sBallMesh->SetWorldTransform(savedBallTransform, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);

        this->sHolderMesh->SetSimulatePhysics(bState);
        this->sTubeMesh->SetSimulatePhysics(bState);
        this->sBallMesh->SetSimulatePhysics(bState);

        this->sHolderMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        this->sTubeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        this->sBallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);

        this->sHolderMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
        this->sTubeMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
        this->sBallMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

        this->sHolderMesh->SetEnableGravity(bState);
        this->sTubeMesh->SetEnableGravity(bState);
        this->sBallMesh->SetEnableGravity(bState);
        this->bPhysicsIsSet = false;
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
                                               false,
                                               ATube::halfTubeHeight * 2,
                                               this->distance);
}

auto ATube::splitBallForce(const double & PIDoutput, const double &inAngle) -> FVector {
    double beta = 180 - (std::abs(inAngle) + 90),
        xForce = 0,
        yForce = 0;
    FVector splitForce;

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
    if ((this->desiredHeight > this->getRegulationHeight()) ||
        (this->desiredHeight < 0))
        UE_LOG(LogTemp, Warning, TEXT("Wrong input! (Expected between 0 and %f)"), this->getRegulationHeight());

    if (!this->pidControllerBall) {
        UE_LOG(LogTemp, Warning, TEXT("UPID for ball not initialized!"));
        return;
    }

    if (!this->bPidBallSwitch) return;

    double error = UPID::estimateError(this->desiredHeight, this->distance),
           output = this->pidControllerBall ? this->pidControllerBall->getPIDOutput(error, (double)deltaTime) : 0;
    //UE_LOG(LogTemp, Warning, TEXT("PID Ball error: = %f"), error);
    //UE_LOG(LogTemp, Warning, TEXT("PID Ball output: = %f"), output);
    if (this->sBallMesh && this->bHit) this->sBallMesh->AddForce(this->splitBallForce(output, this->sTubeMesh->GetComponentRotation().Roll));
}

void ATube::PIDServo(float deltaTime) {
    if (std::abs(this->angle) > this->maxAngle) UE_LOG(LogTemp, Warning, TEXT("Wrong angle! (Expected between -%f and %f)"), this->maxAngle, this->maxAngle);

    if (!this->pidControllerServo) {
        UE_LOG(LogTemp, Warning, TEXT("UPID for servo not initialized!"));
        return;
    }

    FRotator currentRotation = this->sTubeMesh->GetComponentRotation();
    double error = UPID::estimateError(this->angle, currentRotation.Roll),
        output = this->pidControllerServo ? this->pidControllerServo->getPIDOutput(error, (double)deltaTime) : 0;
    
    //UE_LOG(LogTemp, Warning, TEXT("PID desiredAngle: = %f"), this->angle);
    //UE_LOG(LogTemp, Warning, TEXT("PID currentAngle: = %f"), currentRotation.Roll);
    //UE_LOG(LogTemp, Warning, TEXT("PID Servo error: = %f"), error);
    //UE_LOG(LogTemp, Warning, TEXT("PID Servo output: = %f\n"), output);
    if (this->sTubeMesh) this->sTubeMesh->AddTorqueInRadians(FVector(output, 0.0, 0.0), NAME_None, false);
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

void ATube::rotateWidgetToFaceCamera() {
    if (!this->widgetComponent) return;

    FVector cameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
    FVector widgetLocation = this->widgetComponent->GetComponentLocation();

    FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(widgetLocation, cameraLocation);
    lookAtRotation.Pitch = 0.f;
    lookAtRotation.Roll = 0.f; 
    this->widgetComponent->SetWorldRotation(lookAtRotation);
}

void ATube::bindOnButtonSimClick() {
    this->mode = ERunningModesTube::SIMULATION;
    if (this->widgetSwitcher) this->widgetSwitcher->SetActiveWidgetIndex(0);
    this->bChangeMode = true;
}

void ATube::bindOnButtonVizClick() {
    this->mode = ERunningModesTube::VISUALIZATION;
    if (this->widgetSwitcher) this->widgetSwitcher->SetActiveWidgetIndex(1);
    this->bChangeMode = true;
}

void ATube::bindOnButtonSimIRLClick() {
    this->mode = ERunningModesTube::SIM_IN_REAL_TIME;
    if(this->widgetSwitcher) this->widgetSwitcher->SetActiveWidgetIndex(2);
    this->bChangeMode = true;
}

void ATube::bindOnRegCheckboxChange(bool bIsChecked) {
    this->bPidBallSwitch = bIsChecked;
}

void ATube::bindOnidealPIDCheckboxChange(bool bIsChecked) {
    this->bSetIdealBallPID = bIsChecked;
}

void ATube::bindOnCoordsCheckboxChange(bool bIsChecked) {
    this->bCoords = bIsChecked;
}

void ATube::bindOnTextCommitHeight(const FText& Text, ETextCommit::Type CommitType){
    if (CommitType == ETextCommit::OnEnter){
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->setDesiredHeight, *EnteredText);
    }
}

void ATube::bindOnTextCommitAngle(const FText& Text, ETextCommit::Type CommitType) {
    if (CommitType == ETextCommit::OnEnter){
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->setAngle, *EnteredText);
    }
}

void ATube::bindOnTextCommitX(const FText& Text, ETextCommit::Type CommitType) {
    if (CommitType == ETextCommit::OnEnter) {
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->xBallPos, *EnteredText);
    }
}

void ATube::bindOnTextCommitY(const FText& Text, ETextCommit::Type CommitType) {
    if (CommitType == ETextCommit::OnEnter) {
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->yBallPos, *EnteredText);
    }
}

void ATube::bindOnTextCommitP(const FText& Text, ETextCommit::Type CommitType) {
    if (CommitType == ETextCommit::OnEnter) {
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->ballP, *EnteredText);
    }
}

void ATube::bindOnTextCommitI(const FText& Text, ETextCommit::Type CommitType) {
    if (CommitType == ETextCommit::OnEnter) {
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->ballI, *EnteredText);
    }
}

void ATube::bindOnTextCommitD(const FText& Text, ETextCommit::Type CommitType) {
    if (CommitType == ETextCommit::OnEnter) {
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->ballD, *EnteredText);
    }
}

void ATube::setActive(bool bActive){
    this->bIsActive = bActive;
    UE_LOG(LogTemp, Warning, TEXT("Tube now active!"));
}

void ATube::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

    this->widgetComponent->SetVisibility(this->bIsActive);
    
    this->bSetIdealBallPID
        ? 
        this->pidControllerBall->setIdealPIDvalues() 
        : 
        this->pidControllerBall->setPIDvalues(this->ballP,
                                              this->ballI,
                                              this->ballD);

    //UE_LOG(LogTemp, Error, TEXT("Outer kD: %f"), this->servoD);
        this->pidControllerServo->setPIDvalues(this->servoP,
                                               this->servoI,
                                               this->servoD);
       
    this->pidControllerBall->setSaturationLimits(this->saturationLimitBallMin, this->saturationLimitBallMax);
    this->pidControllerServo->setSaturationLimits(this->saturationLimitServoMin, this->saturationLimitServoMax);

    if (ERunningModesTube::SIMULATION == this->mode) {
        if (this->bChangeMode) {
            //UEngineHelper::setKinematicTarget(this->sHolderMesh, false);
            UEngineHelper::setKinematicTarget(this->sTubeMesh, false);
            UEngineHelper::setKinematicTarget(this->sBallMesh, false);
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
            //this->tubeJoint->SetRelativeRotation(FRotator(0, 180, 0));
            this->pidControllerBall->reset();
            this->pidControllerServo->reset();
            this->bChangeMode = false;
        }
        //this->setMeshesPhysicsAndGravity(true);
        this->performRaycast();

        if (!this->bCoords) {
            this->desiredHeight = this->setDesiredHeight;
            this->angle = this->setAngle;
        }
        else {
            this->bPidBallSwitch = true;
            this->convertXYToHeightAngle(this->xBallPos,
                this->yBallPos,
                this->desiredHeight,
                this->angle);

            //UE_LOG(LogTemp, Warning, TEXT("Height: %f Angle: %f"), this->desiredHeight, this->angle);
        }

        this->PIDBall(DeltaTime);
        this->PIDServo(DeltaTime);
        UE_LOG(LogTemp, Display, TEXT("Rotator Tube (Pitch, Yaw, Roll): = (%f, %f, %f)"), this->sTubeMesh->GetRelativeRotation().Pitch, 
                                                                                          this->sTubeMesh->GetRelativeRotation().Yaw, 
                                                                                          this->sTubeMesh->GetRelativeRotation().Roll);

        UE_LOG(LogTemp, Display, TEXT("Rotator Joint (Pitch, Yaw, Roll): = (%f, %f, %f)"), this->tubeJoint->GetRelativeRotation().Pitch,
            this->tubeJoint->GetRelativeRotation().Yaw,
            this->tubeJoint->GetRelativeRotation().Roll);
    }
    else if (ERunningModesTube::VISUALIZATION == this->mode){
        double ballX, 
               ballY;
        
        if (this->bChangeMode){
            //UEngineHelper::setKinematicTarget(this->sHolderMesh, true);
            UEngineHelper::setKinematicTarget(this->sTubeMesh, true);
            UEngineHelper::setKinematicTarget(this->sBallMesh, true);
            this->bChangeMode = false;
        }
        //this->setMeshesPhysicsAndGravity(false);

        this->onReqCompleteFunctor = [this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
            this->funcForWebHandler(request, response, connected);
        };
        this->webHandler->initRequest();
        this->webHandler->setFunctorOnProcessRequestComplete(this, MakeShared<TFunction<void(FHttpRequestPtr, FHttpResponsePtr, bool)>>(MoveTemp(this->onReqCompleteFunctor)));
        this->webHandler->sendRequest();
        
        double testHeight = 20.0,
               testAngle = 45.0;

        this->convertHeightAngleToXY(testHeight, testAngle, ballX, ballY); //this->vizActualHeight, this->angle
        //UE_LOG(LogTemp, Warning, TEXT("height: %f, angle: %f, conX: %f conY: %f"), this->vizActualHeight, this->angle, ballX, ballY);

        this->sBallMesh->SetWorldLocation(FVector(this->GetActorLocation().X - 6.0, this->GetActorLocation().Y + ballX, this->GetActorLocation().Z + ballY));
        this->sTubeMesh->SetWorldLocation(FVector(this->GetActorLocation().X - 6.0, this->GetActorLocation().Y, this->GetActorLocation().Z));
        this->sTubeMesh->SetWorldRotation(FRotator(this->GetActorRotation().Pitch, this->GetActorRotation().Yaw + 180, this->GetActorRotation().Roll + testAngle)); //this->GetActorRotation().Roll + this->angle)
        //this->sTubeMesh->SetWorldRotation(this->GetActorRotation().Add(0.0, 0.0, this->angle));

#if 0
        if (this->reqData.response.IsValid()){
            UE_LOG(LogTemp, Display, TEXT("Response Tube: = %s"), *this->reqData.response->GetContentAsString());
        }
        else {
            UE_LOG(LogTemp, Display, TEXT("Response Tube not valid"));
        }
#endif
    }else{
        if (this->bChangeMode) {
            UEngineHelper::setKinematicTarget(this->sTubeMesh, false);
            UEngineHelper::setKinematicTarget(this->sBallMesh, false);
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
            this->pidControllerBall->reset();
            this->pidControllerServo->reset();
            this->bChangeMode = false;
        }
        this->onReqCompleteFunctor = [this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
            this->funcForWebHandler(request, response, connected);
        };
        this->webHandler->initRequest();
        this->webHandler->setFunctorOnProcessRequestComplete(this, MakeShared<TFunction<void(FHttpRequestPtr, FHttpResponsePtr, bool)>>(MoveTemp(this->onReqCompleteFunctor)));
        this->webHandler->sendRequest();

        this->PIDBall(DeltaTime);
        this->PIDServo(DeltaTime);
    }
    this->rotateWidgetToFaceCamera();
}

#if 0
ATube::~ATube(){
    if (this->pidController) delete this->pidController;
}
#endif