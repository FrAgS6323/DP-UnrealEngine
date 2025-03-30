#pragma once
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Tube.h"
#include "Case.h"
#include "TurtleBody.h"
#include <Components/WidgetInteractionComponent.h>
#include "DigitalTwinLevelScriptActor.generated.h"



UCLASS()
class TUBEUE_API ADigitalTwinLevelScriptActor : public ALevelScriptActor{
	GENERATED_BODY()
	private:
		inline static float cameraSpeed = 1000.0f;
		APlayerController* playerController;
		ATube* tubeModel;
		ACase* caseModel;
		ATurtleBody* turtlebotModel;
	protected:
		UFUNCTION(BlueprintCallable, Category = "LevelBlueprint") void setupWidget();
		UFUNCTION(BlueprintCallable, Category = "LevelBlueprint") void setFocusOnWidget();
		void interactWithActors();
		void interactWithWidgets();
	public:
		ADigitalTwinLevelScriptActor();
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UWidgetInteractionComponent* WidgetInteractor;
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
};