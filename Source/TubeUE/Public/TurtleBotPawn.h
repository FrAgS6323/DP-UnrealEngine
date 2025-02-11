#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "TurtleBody.h"
#include "TurtleWheel.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TurtleBotPawn.generated.h"

//enum class MovementDirection {W, A, S, D};

UCLASS()
class TUBEUE_API ATurtleBotPawn : public APawn{
	GENERATED_BODY()
	private:
		UPROPERTY(EditDefaultsOnly) UCameraComponent* camera;
		void enhancedInputMove(const FInputActionValue &value);
		//MovementDirection movementDir;
	protected:
		virtual void BeginPlay() override;

	public:	
		ATurtleBotPawn();
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true")) TObjectPtr<ATurtleBody> turtleBody;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true")) TObjectPtr<ATurtleWheel> turtleWheelLeft;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true")) TObjectPtr<ATurtleWheel> turtleWheelRight;
		virtual void Tick(float DeltaTime) override;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input") UInputAction* inputToMove;
		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
