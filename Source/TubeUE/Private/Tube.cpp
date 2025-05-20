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

    this->pidControllerBall = MakeUnique<UPID>(ePIDusage::HEIGHT);
    this->pidControllerServo = MakeUnique<UPID>(ePIDusage::ANGLE);

    this->initialize();

#if 0
    TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)> functor = [this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
        this->funcForWebHandler(request, response, connected);
        };
#endif

    this->webHandlerGet = MakeShared<WebHandler>(TEXT("http://147.232.60.231:5001/tubeDataAPI"), WebHandler::eRequestType::GET);
    this->webHandlerPost = MakeShared<WebHandler>(TEXT("http://147.232.60.231:5001/tubeSend"), WebHandler::eRequestType::POST);
}

void ATube::BeginPlay(){
	Super::BeginPlay();
    
    if (!this->widgetComponent) {
        TArray<UWidgetComponent*> WidgetComponents;
        GetComponents<UWidgetComponent>(WidgetComponents);

        for (UWidgetComponent* WidgetComp : WidgetComponents){
            if (WidgetComp->GetName() == TEXT("WidgetForTube")){
                this->widgetComponent = WidgetComp;
                UUserWidget* homeWidget = Cast<UUserWidget>(this->widgetComponent->GetUserWidgetObject());

                if (homeWidget){
                    this->widgetSwitcher = Cast<UWidgetSwitcher>(homeWidget->GetWidgetFromName(TEXT("WidgetSwitcher")));
                    
                    //Simulation
                    UButton* buttonSim = Cast<UButton>(homeWidget->GetWidgetFromName(TEXT("SimulationButton")));
                    buttonSim->OnClicked.AddDynamic(this, &ATube::bindOnButtonSimClick);

                    UButton* buttonViz = Cast<UButton>(homeWidget->GetWidgetFromName(TEXT("VisualizationButton")));
                    buttonViz->OnClicked.AddDynamic(this, &ATube::bindOnButtonVizClick);

                    UButton* buttonSimIRL = Cast<UButton>(homeWidget->GetWidgetFromName(TEXT("SimInRealTimeButton")));
                    buttonSimIRL->OnClicked.AddDynamic(this, &ATube::bindOnButtonSimIRLClick);

                    this->pidCheckBox = Cast<UCheckBox>(homeWidget->GetWidgetFromName(TEXT("bPID")));
                    this->pidCheckBox->OnCheckStateChanged.AddDynamic(this, &ATube::bindOnRegCheckboxChange);

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
                    
                    //Visualization
                    UCheckBox* pidCheckBoxViz = Cast<UCheckBox>(homeWidget->GetWidgetFromName(TEXT("bPIDViz")));
                    pidCheckBoxViz->OnCheckStateChanged.AddDynamic(this, &ATube::bindOnRegCheckboxChangeViz);

                    UCheckBox* bCoordCheckBoxViz = Cast<UCheckBox>(homeWidget->GetWidgetFromName(TEXT("bCoordViz")));
                    bCoordCheckBoxViz->OnCheckStateChanged.AddDynamic(this, &ATube::bindOnCoordsCheckboxChangeViz);

                    UEditableTextBox* editableBoxHeightViz = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("HeightInputViz")));
                    editableBoxHeightViz->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitHeightViz);

                    UEditableTextBox* editableBoxAngleViz = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("AngleInputViz")));
                    editableBoxAngleViz->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitAngleViz);

                    UEditableTextBox* editableBoxXInputViz = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("XInputViz")));
                    editableBoxXInputViz->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitXViz);

                    UEditableTextBox* editableBoxYInputViz = Cast<UEditableTextBox>(homeWidget->GetWidgetFromName(TEXT("YInputViz")));
                    editableBoxYInputViz->OnTextCommitted.AddDynamic(this, &ATube::bindOnTextCommitYViz);

                    this->currentHeightViz = Cast<UTextBlock>(homeWidget->GetWidgetFromName(TEXT("CurrentHeightVarViz")));
                    this->currentAngleViz = Cast<UTextBlock>(homeWidget->GetWidgetFromName(TEXT("CurrentAngleVarViz")));

                    //Simulation IRL
                    this->pidCheckBoxSimIRL = Cast<UCheckBox>(homeWidget->GetWidgetFromName(TEXT("bPIDSimIRL")));
                    this->pidCheckBoxSimIRL->OnCheckStateChanged.AddDynamic(this, &ATube::bindOnRegCheckboxChange);

                    this->currentHeightVarSimIRL = Cast<UTextBlock>(homeWidget->GetWidgetFromName(TEXT("CurrentHeightVarSimIRL")));
                    this->currentAngleVarSimIRL = Cast<UTextBlock>(homeWidget->GetWidgetFromName(TEXT("CurrentAngleVarSimIRL")));
                    this->desiredHeightVarSimIRL = Cast<UTextBlock>(homeWidget->GetWidgetFromName(TEXT("DesiredHeightVarSimIRL")));
                    this->desiredAngleVarSimIRL = Cast<UTextBlock>(homeWidget->GetWidgetFromName(TEXT("DesiredAngleVarSimIRL")));
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

void ATube::funcForWebHandlerGet(FHttpRequestPtr request, FHttpResponsePtr response, bool connected){
    if (response.IsValid()) {
        TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(response->GetContentAsString());
        FJsonSerializer::Deserialize(jsonReader, this->responseObj);

        FString value;
        if (this->responseObj->TryGetStringField("target", value)) LexTryParseString(this->desiredHeight, *value);
        if (this->responseObj->TryGetStringField("height", value)) {
            LexTryParseString(this->actualHeightViz, *value);
            //UE_LOG(LogTemp, Warning, TEXT("VizHeight Response: %f"), this->actualHeightViz);
            if (this->actualHeightViz <= 8.0) this->actualHeightViz = 0.0;
        }
        if (this->responseObj->TryGetStringField("angle", value)) LexTryParseString(this->angle, *value);
    }
}

void ATube::funcForWebHandlerPost(FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
    if (response.IsValid()) {
        UE_LOG(LogTemp, Warning, TEXT("POST Response: %s"), *response->GetContentAsString());
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("No response received"));
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

auto ATube::getDistance() -> double {
	return (this->distance > 0.001) ? this->distance : 0;
}

auto ATube::getRegulationHeight() -> double {
    return 45.0f;
}

auto ATube::getBottomCoordsOfTube(double actualAngle) -> FVector{
    //distanceFromTubeOriginToBottom = (this->height - this->motorHeight - this->upperHeight) / 2,
    double angleDegToSplit = 0,
        raycastDistanceFromOriginX = 0,
        raycastDistanceFromOriginY = ATube::halfTubeHeight; //distanceFromTubeOriginToBottom;
    FVector bottomPos(0, 0, 0);

    if (0 != actualAngle) {
        if (actualAngle > 0) angleDegToSplit = 360 - actualAngle;
        else angleDegToSplit = 180 + std::abs(actualAngle);

        //UE_LOG(LogTemp, Warning, TEXT("angleDegToSplit: %f"), angleDegToSplit);

        raycastDistanceFromOriginX = ATube::halfTubeHeight * cos(UEngineHelper::degToRad(angleDegToSplit));
        raycastDistanceFromOriginY = ATube::halfTubeHeight * sin(UEngineHelper::degToRad(angleDegToSplit));

        bottomPos.Y = -raycastDistanceFromOriginX;
        bottomPos.Z = -std::abs(raycastDistanceFromOriginY);
    }
    else {
        bottomPos.Z = -ATube::halfTubeHeight;
    }
    return bottomPos;
}

void ATube::performRaycast() {
    FVector raycastPos = this->getBottomCoordsOfTube(this->sTubeMesh->GetComponentRotation().Roll - 90);
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
    if (0 == inHeight) inHeight = 1.5; // ak je inHeight = 0, tak lopticka sa ocitne trochu mimo trubice!
    //if(height != 0){
    if (inAngle < 0) {
        inAngle *= (-1);
        xm = 1;
    }
    //inAngle = (std::numbers::pi / 180) * (inAngle);
    inAngle = UEngineHelper::degToRad(inAngle);

    c = static_cast<double>(inHeight) - ATube::halfTubeHeight;
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
        y = - ATube::halfTubeHeight; // -20
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
    this->bPidBallSwitch = false;
    this->pidCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
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
    this->bPidBallSwitch = false;
    this->pidCheckBoxSimIRL->SetCheckedState(ECheckBoxState::Unchecked);
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

void ATube::bindOnRegCheckboxChangeViz(bool bIsChecked){
    this->bPidBallSwitchViz = bIsChecked;

    FtubePost data;
    data.power = this->bPidBallSwitchViz;

    this->webHandlerPost->setPostReqData(data);
    this->webHandlerPost->initRequest({"power", "_"});
    this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
    this->webHandlerPost->sendRequest();
}

void ATube::bindOnCoordsCheckboxChangeViz(bool bIsChecked){
    this->bCoordsViz = bIsChecked;
}

void ATube::bindOnTextCommitHeightViz(const FText& Text, ETextCommit::Type CommitType){
    if (CommitType == ETextCommit::OnEnter) {
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->setDesiredHeightViz, *EnteredText);

        FtubePost data;
        data.height = this->setDesiredHeightViz;
        data.angle = this->setAngleViz;

        this->webHandlerPost->setPostReqData(data);
        this->webHandlerPost->initRequest({"height", "angle"});
        this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
        this->webHandlerPost->sendRequest();
    }
}

void ATube::bindOnTextCommitAngleViz(const FText& Text, ETextCommit::Type CommitType){
    if (CommitType == ETextCommit::OnEnter) {
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->setAngleViz, *EnteredText);

        FtubePost data;
        data.height = this->setDesiredHeightViz;
        data.angle = this->setAngleViz;

        this->webHandlerPost->setPostReqData(data);
        this->webHandlerPost->initRequest({ "height", "angle" });
        this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
        this->webHandlerPost->sendRequest();
    }
}

void ATube::bindOnTextCommitXViz(const FText& Text, ETextCommit::Type CommitType){
    if (CommitType == ETextCommit::OnEnter) {
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->xBallPosViz, *EnteredText);
        
        if (!this->bCoordsViz){
            UE_LOG(LogTemp, Warning, TEXT("Visualization Coordinates mode needs to be turned on!"));
            return;
        }

        FtubePost data;
        data.X = this->xBallPosViz;
        data.Y = this->yBallPosViz;

        this->webHandlerPost->setPostReqData(data);
        this->webHandlerPost->initRequest({ "X", "Y" });
        this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
        this->webHandlerPost->sendRequest();
    }
}

void ATube::bindOnTextCommitYViz(const FText& Text, ETextCommit::Type CommitType){
    if (CommitType == ETextCommit::OnEnter) {
        FString EnteredText = Text.ToString().TrimStartAndEnd();
        LexTryParseString(this->yBallPosViz, *EnteredText);
        
        if (!this->bCoordsViz) {
            UE_LOG(LogTemp, Warning, TEXT("Visualization Coordinates mode needs to be turned on!"));
            return;
        }

        FtubePost data;
        data.X = this->xBallPosViz;
        data.Y = this->yBallPosViz;

        this->webHandlerPost->setPostReqData(data);
        this->webHandlerPost->initRequest({ "X", "Y" });
        this->webHandlerPost->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorPost);
        this->webHandlerPost->sendRequest();
    }
}

void ATube::setActive(APlayerController * inPlayerController, bool bActive){
    this->bIsActive = bActive;
    this->playerController = this->bIsActive ? inPlayerController : nullptr;
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

        this->pidControllerServo->setPIDvalues(this->servoP,
                                               this->servoI,
                                               this->servoD);
       
    this->pidControllerBall->setSaturationLimits(this->saturationLimitBallMin, this->saturationLimitBallMax);
    this->pidControllerServo->setSaturationLimits(this->saturationLimitServoMin, this->saturationLimitServoMax);

    if (ERunningModesTube::SIMULATION == this->mode) {
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
    }
    else if (ERunningModesTube::VISUALIZATION == this->mode){
        double ballX, 
               ballY;
        
        if (this->bChangeMode){
            UEngineHelper::setKinematicTarget(this->sTubeMesh, true);
            UEngineHelper::setKinematicTarget(this->sBallMesh, true);
            this->bChangeMode = false;
        }

        this->onReqCompleteFunctorGet =MakeShared<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>> ([this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
            this->funcForWebHandlerGet(request, response, connected);

            this->webHandlerGet->initRequest();
            this->webHandlerGet->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorGet);
            this->webHandlerGet->sendRequest();
        });

        if (!this->bFirstReqGETSent) {
            this->webHandlerGet->initRequest();
            this->webHandlerGet->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorGet);
            this->webHandlerGet->sendRequest();
            this->bFirstReqGETSent = true;
        }
        
        //double testHeight = 0.0,
               //testAngle = -45.0;
        
        if (this->currentHeightViz) this->currentHeightViz->SetText(FText::FromString(FString::SanitizeFloat(this->actualHeightViz))); else  UE_LOG(LogTemp, Warning, TEXT("height textBlock is null!"));
        if (this->currentAngleViz) this->currentAngleViz->SetText(FText::FromString(FString::SanitizeFloat(this->angle))); else UE_LOG(LogTemp, Warning, TEXT("angle textBlock is null!"));

        this->convertHeightAngleToXY(this->actualHeightViz, this->angle, ballX, ballY);
        //this->convertHeightAngleToXY(testHeight, testAngle, ballX, ballY);
        //UE_LOG(LogTemp, Warning, TEXT("height: %f, angle: %f, conX: %f conY: %f"), this->actualHeightViz, this->angle, ballX, ballY);

        this->sBallMesh->SetWorldLocation(FVector(this->GetActorLocation().X - 6.0, this->GetActorLocation().Y + ballX, this->GetActorLocation().Z + ballY));
        this->sTubeMesh->SetWorldLocation(FVector(this->GetActorLocation().X - 6.0, this->GetActorLocation().Y, this->GetActorLocation().Z));
        this->sTubeMesh->SetWorldRotation(FRotator(this->GetActorRotation().Pitch, this->GetActorRotation().Yaw + 180, this->GetActorRotation().Roll + this->angle)); //this->GetActorRotation().Roll + this->angle)
        //this->sTubeMesh->SetWorldRotation(this->GetActorRotation().Add(0.0, 0.0, this->angle));
        
        this->onReqCompleteFunctorPost = MakeShared<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>>([this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
            this->funcForWebHandlerPost(request, response, connected);
        });

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
        this->onReqCompleteFunctorGet = MakeShared<TFunction<void(FHttpRequestPtr request, FHttpResponsePtr response, bool connected)>>([this](FHttpRequestPtr request, FHttpResponsePtr response, bool connected) {
            this->funcForWebHandlerGet(request, response, connected);
        });
        this->webHandlerGet->initRequest();
        this->webHandlerGet->setFunctorOnProcessRequestComplete(this, this->onReqCompleteFunctorGet);
        this->webHandlerGet->sendRequest();
        
        this->performRaycast();
        this->PIDBall(DeltaTime);
        this->PIDServo(DeltaTime);
        
        if (this->desiredHeightVarSimIRL) this->desiredHeightVarSimIRL->SetText(FText::FromString(FString::SanitizeFloat(this->desiredHeight))); else  UE_LOG(LogTemp, Warning, TEXT("Desired height textBlock Sim IRL is null!"));
        if (this->desiredAngleVarSimIRL) this->desiredAngleVarSimIRL->SetText(FText::FromString(FString::SanitizeFloat(this->angle))); else UE_LOG(LogTemp, Warning, TEXT("Desired angle textBlock Sim IRL is null!"));
        if (this->currentHeightVarSimIRL) this->currentHeightVarSimIRL->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), this->distance))); else  UE_LOG(LogTemp, Warning, TEXT("Current height textBlock Sim IRL is null!"));
        if (this->currentAngleVarSimIRL) this->currentAngleVarSimIRL->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), this->sTubeMesh->GetComponentRotation().Roll))); else UE_LOG(LogTemp, Warning, TEXT("Current angle textBlock Sim IRL is null!"));
    }
    this->rotateWidgetToFaceCamera();

    if (this->playerController && this->playerController->WasInputKeyJustReleased(EKeys::U)) this->bShowColliders = !this->bShowColliders;
    if(this->bShowColliders) UEngineHelper::drawAllSimpleCollidersForActor(this);
}