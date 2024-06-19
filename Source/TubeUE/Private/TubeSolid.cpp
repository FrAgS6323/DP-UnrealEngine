#include "TubeSolid.h"

// Sets default values
ATubeSolid::ATubeSolid(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->tubeSolidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TubeSolidMesh"));
	RootComponent = this->tubeSolidMesh;
	this->rotateTubeComponent = CreateDefaultSubobject<URotateTube>(TEXT("RotateTubeComponent"));

	this->tubeSolidMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ATubeSolid::BeginPlay(){
	Super::BeginPlay();

	FVector meshBounds = this->tubeSolidMesh->Bounds.BoxExtent;
	FVector centerOfMassOffset = FVector(0.0f, 0.0f, -meshBounds.Z); // Posun ťažiska na stred

	this->tubeSolidMesh->SetCenterOfMass(centerOfMassOffset, NAME_None);
}

// Called every frame
void ATubeSolid::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	//if (this->rotateTubeComponent){
		//this->rotateTubeComponent->rotate(this->angle);
	//}
}