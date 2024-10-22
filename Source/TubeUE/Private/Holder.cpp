#include "Holder.h"
#include "EngineUtils.h"

AHolder::AHolder() {
	PrimaryActorTick.bCanEverTick = true;
	this->sHolderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Holder"));
	//this->physicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	RootComponent = this->sHolderMesh;
	this->sHolderMesh->SetSimulatePhysics(false);
}

void AHolder::BeginPlay() {
	Super::BeginPlay();

	FVector centerOfMassOffset = FVector(0.0f, 0.0f, 0.0f);
	this->sHolderMesh->SetCenterOfMass(centerOfMassOffset, NAME_None);

	for (TActorIterator<ATube> it(GetWorld()); it; ++it) {
		this->tube = *it;
		break;
	}
#if 0
	if(this->tube)
		this->tubeRootComponent = Cast<UPrimitiveComponent>(this->tube->GetRootComponent());
	
	if (this->physicsConstraint) {
		this->physicsConstraint->SetConstrainedComponents(this->sHolderMesh, "Holder", this->tubeRootComponent, "Tube");
		this->physicsConstraint->bVisualizeComponent = true;
		// Lock the linear movement
		this->physicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
		this->physicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
		this->physicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

		// Allow free angular motion
		this->physicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);  // Pitch
		this->physicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);  // Roll
		this->physicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);   // Yaw
	}
#endif
}

void AHolder::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}