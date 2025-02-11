#include "TurtleBotController.h"

// Constructor
ATurtleBotController::ATurtleBotController(){
    // Set default values (optional)
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATurtleBotController::BeginPlay(){
    Super::BeginPlay();
#if 0
    this->controlledTurtle = Cast<ATurtleBody>(Super::GetPawn());

    if (!this->controlledTurtle)
        UE_LOG(LogTemp, Warning, TEXT("TurtleBotController: No TurtleBody Pawn is being controlled!"));

    this->setupInputComponent(this->controlledTurtle->InputComponent);
#endif
}

// Setup Player Input
void ATurtleBotController::setupInputComponent(UInputComponent* inputComponent) {


    if (!inputComponent) {
        UE_LOG(LogTemp, Error, TEXT("TurtleBot InputComponent returned nullptr"));
        return;
    }
        // Bind movement actions
        inputComponent->BindAxis("MoveForward", this, &ATurtleBotController::moveForward);
        inputComponent->BindAxis("TurnRight", this, &ATurtleBotController::turnRight);
}

// Function to get a component by name
UStaticMeshComponent* ATurtleBotController::findComponentByName(AActor* owner, const FName& componentName) {
    if (!owner) return nullptr;

    // Iterate through all components of the actor
    TArray<UActorComponent*> Components;
    Owner->GetComponents(UStaticMeshComponent::StaticClass(), Components);

    for (UActorComponent* Component : Components){
        if (Component->GetName() == componentName.ToString())
            return Cast<UStaticMeshComponent>(Component);
    }
    return nullptr; // Return nullptr if no matching component is found
}

// Input: Move Forward/Backward
void ATurtleBotController::moveForward(float value){
    if (this->controlledTurtle){
        // Both wheels rotate in the same direction for forward/backward movement
        this->rotateWheels(value, value);
    }
}

// Input: Move Right/Left
void ATurtleBotController::turnRight(float value){
    if (this->controlledTurtle) {
        this->rotateWheels(-value, value);
    }
}

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

void ATurtleBotController::rotateWheels(float leftWheelSpeed, float rightWheelSpeed) {
    if (!this->controlledTurtle) return;

    UStaticMeshComponent* sMeshLeftWheel = this->findComponentByName(this->controlledTurtle, "TurtleWheelLeft");
    UStaticMeshComponent* sMeshRightWheel = this->findComponentByName(this->controlledTurtle, "TurtleRightLeft");

    if (sMeshLeftWheel) {
        FVector localAngularVelocity = FVector(0.0, leftWheelSpeed, 0.0);
        //UE_LOG(LogTemp, Warning, TEXT("localAngularVelocity: %s"), *localAngularVelocity.ToString());
        FVector worldAngularVelocity = sMeshLeftWheel->GetComponentTransform().TransformVector(localAngularVelocity);
        //UE_LOG(LogTemp, Warning, TEXT("worldAngularVelocity: %s"), *worldAngularVelocity.ToString());
        sMeshLeftWheel->SetPhysicsAngularVelocityInDegrees(worldAngularVelocity, false);
    }
    if (sMeshRightWheel) {
        FVector localAngularVelocity = FVector(0.0, rightWheelSpeed, 0.0);
        //UE_LOG(LogTemp, Warning, TEXT("localAngularVelocity: %s"), *localAngularVelocity.ToString());
        FVector worldAngularVelocity = sMeshRightWheel->GetComponentTransform().TransformVector(localAngularVelocity);
        //UE_LOG(LogTemp, Warning, TEXT("worldAngularVelocity: %s"), *worldAngularVelocity.ToString());
        sMeshRightWheel->SetPhysicsAngularVelocityInDegrees(worldAngularVelocity, false);
    }
}

// Called every frame
void ATurtleBotController::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
}