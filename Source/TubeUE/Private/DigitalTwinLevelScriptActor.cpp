#include "DigitalTwinLevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/EditableTextBox.h"
#include "Framework/Application/SlateApplication.h"
#include "../EngineHelper.h"

ADigitalTwinLevelScriptActor::ADigitalTwinLevelScriptActor(){
    WidgetInteractor = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractor"));
    WidgetInteractor->SetupAttachment(RootComponent);
    WidgetInteractor->InteractionDistance = 1000.0f;
    WidgetInteractor->TraceChannel = ECC_Visibility;
    WidgetInteractor->bShowDebug = false;
    WidgetInteractor->bEnableHitTesting = true;
    WidgetInteractor->InteractionSource = EWidgetInteractionSource::Mouse;
}

void ADigitalTwinLevelScriptActor::BeginPlay(){
    Super::BeginPlay();
    
    UEngineHelper::Profiler::initCPUprofiler();
    this->playerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!this->playerController) return;

    this->playerController->bShowMouseCursor = true;
    this->playerController->SetInputMode(FInputModeGameAndUI());
    UEngineHelper::setSpectatorCameraSpeed(this->playerController, ADigitalTwinLevelScriptActor::cameraSpeed);
    
    this->tubeModel = Cast<ATube>(UEngineHelper::findActorsOfClass(this, ATube::StaticClass(), 0));
    this->caseModel = Cast<ACase>(UEngineHelper::findActorsOfClass(this, ACase::StaticClass(), 0));
    this->turtlebotModel = Cast<ATurtleBody>(UEngineHelper::findActorsOfClass(this, ATurtleBody::StaticClass(), 0));
}

void ADigitalTwinLevelScriptActor::interactWithActors(){
    if (this->playerController->WasInputKeyJustReleased(EKeys::LeftMouseButton)) {
        FVector WorldPos, WorldDir;
        this->playerController->DeprojectMousePositionToWorld(WorldPos, WorldDir);

        FHitResult Hit;
        FVector End = WorldPos + WorldDir * 1000.0f;
        this->GetWorld()->LineTraceSingleByChannel(Hit, WorldPos, End, ECC_Visibility);
        //DrawDebugLine(GetWorld(), WorldPos, End, FColor::Red, false, 1, 0, 1);

        if (Hit.GetActor() == this->tubeModel){
            //this->tubeModel->widgetComponent->SetVisibility(true);
            this->tubeModel->setActive(true);
            this->turtlebotModel->setActive(nullptr, false);
            UEngineHelper::setSpectatorCameraSpeed(this->playerController, ADigitalTwinLevelScriptActor::cameraSpeed);
        }
        else if (Hit.GetActor() == this->caseModel) {
            //this->tubeModel->widget->SetVisibility(true);
            this->turtlebotModel->setActive(nullptr, false);
            UEngineHelper::setSpectatorCameraSpeed(this->playerController, ADigitalTwinLevelScriptActor::cameraSpeed);
        }
        else if (Hit.GetActor() == this->turtlebotModel) {
            //this->tubeModel->widget->SetVisibility(true);
            this->turtlebotModel->setActive(this->playerController, true);
            UEngineHelper::setSpectatorCameraSpeed(this->playerController, 0.0f);
        }
        else{
            //this->tubeModel->widgetComponent->SetVisibility(false);
            this->tubeModel->setActive(false);
            this->turtlebotModel->setActive(nullptr, false);
            UEngineHelper::setSpectatorCameraSpeed(this->playerController, ADigitalTwinLevelScriptActor::cameraSpeed);
            UE_LOG(LogTemp, Warning, TEXT("No model active"));
        }
    }
}

void ADigitalTwinLevelScriptActor::interactWithWidgets(){
    FVector mouseWorldPos, WorldDir;
    this->playerController->DeprojectMousePositionToWorld(mouseWorldPos, WorldDir);

    FHitResult HitResult;
    FVector End = mouseWorldPos + WorldDir * 1000.0f;

    WidgetInteractor->SetWorldLocation(mouseWorldPos);
    WidgetInteractor->SetWorldRotation(mouseWorldPos.Rotation());

    WidgetInteractor->TickComponent(0.f, ELevelTick::LEVELTICK_TimeOnly, nullptr);
}

void ADigitalTwinLevelScriptActor::Tick(float DeltaTime) {
    this->interactWithWidgets();
    this->interactWithActors();
    UEngineHelper::Profiler::profilePerformance(true, DeltaTime);
    
    if (this->bOnce && this->numOfTicksToProfile < this->tickCount) {
        UEngineHelper::Profiler::writeAvgsToLOG();
        this->bOnce = false;
    }
    this->tickCount++;
}

void ADigitalTwinLevelScriptActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    //UEngineHelper::Profiler::writeAvgsToLOG();
}