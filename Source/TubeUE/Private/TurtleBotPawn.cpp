#include "TurtleBotPawn.h"
#include "EnhancedInputComponent.h"

ATurtleBotPawn::ATurtleBotPawn(){
	PrimaryActorTick.bCanEverTick = true;
	this->turtleBody = Cast<ATurtleBody>(GetDefaultSubobjectByName(TEXT("TurtlebotBody")));

	if (this->turtleBody) {
		UE_LOG(LogTemp, Warning, TEXT("TurtleBody loaded"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("TurtleBody not loaded"));
	}
}

// Called when the game starts or when spawned
void ATurtleBotPawn::BeginPlay(){
	Super::BeginPlay();
#if 0
	this->camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TurtlebotBodyCamera"));

	this->turtleBody = Cast<ATurtleBody>(GetDefaultSubobjectByName(TEXT("TurtleBody")));
	this->turtleWheelLeft = Cast<ATurtleWheel>(GetDefaultSubobjectByName(TEXT("TurtleWheelLeft")));
	this->turtleWheelRight = Cast<ATurtleWheel>(GetDefaultSubobjectByName(TEXT("TurtleWheelRight")));
	if(this->turtleBody) this->RootComponent = this->turtleBody->GetRootComponent();
	this->camera->SetupAttachment(this->turtleBody->GetRootComponent());
#endif
}

// Called to bind functionality to input
void ATurtleBotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent *enhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	//enhancedInput->BindAction(this->inputToMove, ETriggerEvent::Triggered, this, &ATurtleBotPawn::enhancedInputMove);
}

void ATurtleBotPawn::enhancedInputMove(const FInputActionValue& value){
	

	FVector2D moveVec = value.Get<FVector2D>();
	FRotator moveRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	
	if (!turtleWheelLeft || !turtleWheelRight){
		UE_LOG(LogTemp, Warning, TEXT("Wheels not attached properly!"));
		return;
	}
	auto leftWheelMesh = Cast<UStaticMeshComponent>(this->turtleWheelLeft->GetRootComponent());
	auto rightWheelMesh = Cast<UStaticMeshComponent>(this->turtleWheelRight->GetRootComponent());

	auto worldToLocalAxes = [](ATurtleWheel* wheel, bool inverse) -> FVector {
		constexpr double rotationVel = 1000;
		FRotator rotation = wheel->GetActorRotation();

		FVector localAngularVelocity = FVector(0.0, inverse ? -rotationVel : rotationVel, 0.0);
		FVector worldAngularVelocity = wheel->GetRootComponent()->GetComponentTransform().TransformVector(localAngularVelocity);
		return worldAngularVelocity;
	};

	if (leftWheelMesh && rightWheelMesh) {
		if (moveVec.X > 0.05f){
			leftWheelMesh->SetPhysicsAngularVelocityInDegrees(worldToLocalAxes(this->turtleWheelLeft, false), false);
			rightWheelMesh->SetPhysicsAngularVelocityInDegrees(worldToLocalAxes(this->turtleWheelRight, false), false);
		}
		else if (moveVec.X < 0.05f){
			leftWheelMesh->SetPhysicsAngularVelocityInDegrees(worldToLocalAxes(this->turtleWheelLeft, true), false);
			rightWheelMesh->SetPhysicsAngularVelocityInDegrees(worldToLocalAxes(this->turtleWheelRight, true), false);
		}
		else if (moveVec.Y > 0.05f) {
			leftWheelMesh->SetPhysicsAngularVelocityInDegrees(worldToLocalAxes(this->turtleWheelLeft, true), false);
			rightWheelMesh->SetPhysicsAngularVelocityInDegrees(worldToLocalAxes(this->turtleWheelRight, false), false);
		}
		else if (moveVec.Y < 0.05f) {
			leftWheelMesh->SetPhysicsAngularVelocityInDegrees(worldToLocalAxes(this->turtleWheelLeft, false), false);
			rightWheelMesh->SetPhysicsAngularVelocityInDegrees(worldToLocalAxes(this->turtleWheelRight, true), false);
		}
	}
}

// Called every frame
void ATurtleBotPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}