#pragma once

#include "CoreMinimal.h"
#include "TurtleBody.h"
#include "GameFramework/PlayerController.h"
#include "TurtleBotController.generated.h"


UCLASS()
class TUBEUE_API ATurtleBotController : public APlayerController{
	GENERATED_BODY()
    
    private:
        double rotationVel;
        static constexpr double rotationVelForward = 1000;
        static constexpr double rotationVelStop = 1;
        ATurtleBody* controlledTurtle;
        void setupInputComponent(UInputComponent* inputComponent);
        UStaticMeshComponent* findComponentByName(AActor* owner, const FName& componentName);
    protected:
        // Called when the game starts or when spawned
        virtual void BeginPlay() override;

        // Input Functions
        void moveForward(float value);
        void turnRight(float value); 
    public:
        ATurtleBotController();
        void rotateWheels(float leftWheelSpeed, float rightWheelSpeed);
        // Called every frame
        virtual void Tick(float DeltaTime) override;
};