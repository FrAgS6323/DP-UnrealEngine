#include "TurtleLidarTop.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATurtleLidarTop::ATurtleLidarTop(){
	this->sBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotLidarTop"));
	RootComponent = this->sBodyMesh;
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Models/TurtleBot/TurtlebotLidarTop"));
	UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Models/TurtleBot/TurtlebotLidarTop")));
	if (MeshAsset)
	{
		this->sBodyMesh->SetStaticMesh(MeshAsset);
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully: %s"), *MeshAsset->GetName());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Mesh not loaded!"));
	}
}

// Called when the game starts or when spawned
void ATurtleLidarTop::BeginPlay(){
	Super::BeginPlay();
	this->sBodyMesh->SetMobility(EComponentMobility::Movable);
	this->sBodyMesh->SetSimulatePhysics(false);
	this->sBodyMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	this->sBodyMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

// Called every frame
void ATurtleLidarTop::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}