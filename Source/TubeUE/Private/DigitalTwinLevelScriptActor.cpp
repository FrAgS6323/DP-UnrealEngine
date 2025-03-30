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
    this->setupWidget();
    UEngineHelper::setSpectatorCameraSpeed(this->playerController, ADigitalTwinLevelScriptActor::cameraSpeed);
}

void ADigitalTwinLevelScriptActor::setupWidget(){
    this->playerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!this->playerController) return;

    this->playerController->bShowMouseCursor = true;
    this->playerController->SetInputMode(FInputModeGameAndUI());

    this->tubeModel = Cast<ATube>(UEngineHelper::findActorsOfClass(this, ATube::StaticClass(), 0));
    this->caseModel = Cast<ACase>(UEngineHelper::findActorsOfClass(this, ACase::StaticClass(), 0));
    this->turtlebotModel = Cast<ATurtleBody>(UEngineHelper::findActorsOfClass(this, ATurtleBody::StaticClass(), 0));
    // Príklad použitia widgetu (2D na obrazovke)
    /*
    if (UUserWidget* Widget = CreateWidget(this->playerController, UYourWidgetClass::StaticClass()))
    {
        Widget->AddToViewport();
    }
    */
}

void ADigitalTwinLevelScriptActor::setFocusOnWidget(){
    if (this->tubeModel && this->tubeModel->widgetComponent){
        UUserWidget* tubeWidget = this->tubeModel->widgetComponent->GetUserWidgetObject();
        if (!tubeWidget){
            UE_LOG(LogTemp, Warning, TEXT("Tube Widget not referenced in level!"));
            return;
        }

        UEditableTextBox* editableTextBox = Cast<UEditableTextBox>(tubeWidget->GetWidgetFromName(TEXT("HeightInput")));

        if (!editableTextBox) {
            UE_LOG(LogTemp, Warning, TEXT("Editable textbox not referenced in level!"));
            return;
        }

        if (this->playerController->WasInputKeyJustReleased(EKeys::LeftMouseButton)) {
            FVector WorldPos, WorldDir;
            this->playerController->DeprojectMousePositionToWorld(WorldPos, WorldDir);

            FHitResult Hit;
            FVector End = WorldPos + WorldDir * 1000.0f;
            GetWorld()->LineTraceSingleByChannel(Hit, WorldPos, End, ECC_Visibility);

            if (Hit.GetActor() == this->tubeModel && Hit.Component == this->tubeModel->widgetComponent) {
                if (tubeWidget) {
                    if (editableTextBox)
                        UE_LOG(LogTemp, Warning, TEXT("Widget is hit"));
                        editableTextBox->SetKeyboardFocus();
                    }
                }
            }
            else{
                FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);
                UE_LOG(LogTemp, Warning, TEXT("Missed"));
            }
        }

            //UE_LOG(LogTemp, Warning, TEXT("Has keyboard focus? %s"), editableTextBox->HasKeyboardFocus() ? TEXT("YES") : TEXT("NO"));
            //UE_LOG(LogTemp, Warning, TEXT("Has user focus? %s"), editableTextBox->HasUserFocus(this->playerController) ? TEXT("YES") : TEXT("NO"));
#if 0
            if (UEditableTextBox* editableTextBox = Cast<UEditableTextBox>(widget->GetWidgetFromName(TEXT("HeightInput"))))
            {
                if (editableTextBox->HasKeyboardFocus())
                {
                        
                    UE_LOG(LogTemp, Warning, TEXT("Editable textbox is already focused"));
                }
                else{
                    editableTextBox->SetFocus();
                    UE_LOG(LogTemp, Warning, TEXT("Editable textbox set to focus"));
                }
            }
            else{
                UE_LOG(LogTemp, Warning, TEXT("Editable textbox not loaded!"));
            }
#endif
        //}
        //else{
            //UE_LOG(LogTemp, Warning, TEXT(" Right MB Clicked"));
        //}
    //}
    else{
        UE_LOG(LogTemp, Warning, TEXT("tube not initialized in level!"));
    }
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

    //FHitResult CursorHit;
    //this->playerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, CursorHit);

#if 0
    if (CursorHit.GetComponent()) {
        UE_LOG(LogTemp, Warning, TEXT("Hit result: %s"), *CursorHit.GetComponent()->GetName());
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("No component hit by cursor"));
    }
#endif 

#if 0
    if (this->playerController->WasInputKeyJustReleased(EKeys::LeftMouseButton)){
        if (bIsHit)
        {
            if (HitResult.Component.IsValid()) {
                UE_LOG(LogTemp, Warning, TEXT("Hit component: %s (class: %s)"),
                    *HitResult.Component->GetName(),
                    *HitResult.Component->GetClass()->GetName());
            }

            auto WidgetComponent = Cast<UWidgetComponent>(HitResult.Component.Get());
            if (WidgetComponent) {
                UUserWidget* Widget = WidgetComponent->GetUserWidgetObject();
                UE_LOG(LogTemp, Warning, TEXT("Hovered widget: %s"), *Widget->GetName());
            }
            else{
                UE_LOG(LogTemp, Warning, TEXT("widget cast failed"));
            }

            if (WidgetInteractor->IsOverFocusableWidget())
            {
                // Simulate click on widget
                //WidgetInteractor->PressPointerKey(EKeys::LeftMouseButton);
                //WidgetInteractor->ReleasePointerKey(EKeys::LeftMouseButton);
                UE_LOG(LogTemp, Warning, TEXT("Clicked inside widget (focusable)!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Clicked widget but no focusable subwidget under pointer"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No widget under mouse"));
        }
    }
#endif
}

void ADigitalTwinLevelScriptActor::Tick(float DeltaTime){
    //this->setFocusOnWidget();
    this->interactWithWidgets();
    this->interactWithActors();
#if 0
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &ADigitalTwinLevelScriptActor::setFocusOnWidget,
        0.05f, // alebo aj 0.01f
        false
    );
#endif
}