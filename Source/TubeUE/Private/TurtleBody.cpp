#include "TurtleBody.h"

// Sets default values
ATurtleBody::ATurtleBody(){
	this->sBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurtlebotBody"));
    if (this->sBodyMesh) RootComponent = this->sBodyMesh;
    else UE_LOG(LogTemp, Error, TEXT("RootComponent not initialized!"));
	PrimaryActorTick.bCanEverTick = true;

    UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Models/TurtleBot/TurtlebotBody")));
    if (MeshAsset){
        UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully: %s"), *MeshAsset->GetName());
        this->sBodyMesh->SetStaticMesh(MeshAsset);
    }else{
        UE_LOG(LogTemp, Error, TEXT("Mesh not loaded!"));
    }
}

// Called when the game starts or when spawned
void ATurtleBody::BeginPlay(){
	Super::BeginPlay();
    if (this->sBodyMesh) {
        this->sBodyMesh->SetMobility(EComponentMobility::Movable);
        this->sBodyMesh->SetSimulatePhysics(true);
        this->sBodyMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
    }
    //this->attachActors();
}

void ATurtleBody::attachActors(){
    // Check the world context
    UWorld* world = GetWorld();
    if (!world) {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() returned nullptr"));
        return;
    }

    if (world) {
        // Spawn the child actor
        FActorSpawnParameters spawnParams;
        spawnParams.Owner = this;
        spawnParams.Instigator = GetInstigator();

        FVector actorLocation = this->GetActorLocation();
        double x = actorLocation.X,
            y = actorLocation.Y,
            z = actorLocation.Z;

        this->dSubWheelLeft = world->SpawnActor<ATurtleWheel>(ATurtleWheel::StaticClass(), FVector(x + 100, y ,z), FRotator::ZeroRotator, spawnParams);
        this->dSubWheelRight = world->SpawnActor<ATurtleWheel>(ATurtleWheel::StaticClass(), FVector(x - 100, y, z), FRotator::ZeroRotator, spawnParams);
        this->dSubLidarTop = world->SpawnActor<ATurtleLidarTop>(ATurtleLidarTop::StaticClass(), FVector(x, y + 100, z), FRotator::ZeroRotator, spawnParams);
        this->dSubLidarBottom = world->SpawnActor<ATurtleLidarBottom>(ATurtleLidarBottom::StaticClass(), FVector(x, y - 100, z), FRotator::ZeroRotator, spawnParams);

        if (this->dSubWheelLeft) {
            this->dSubWheelLeft->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            UE_LOG(LogTemp, Warning, TEXT("Actor attached successfully to parent"));
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("SpawnActor failed to create the actor"));
            return;
        }

        if (this->dSubWheelRight) {
            this->dSubWheelRight->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            UE_LOG(LogTemp, Warning, TEXT("Actor attached successfully to parent"));
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("SpawnActor failed to create the actor"));
            return;
        }

        if (this->dSubLidarTop) {
            this->dSubLidarTop->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            UE_LOG(LogTemp, Warning, TEXT("Actor attached successfully to parent"));
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("SpawnActor failed to create the actor"));
            return;
        }

        if (this->dSubLidarBottom) {
            this->dSubLidarBottom->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            UE_LOG(LogTemp, Warning, TEXT("Actor attached successfully to parent"));
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("SpawnActor failed to create the actor"));
            return;
        }
    }
}

// Called every frame
void ATurtleBody::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}