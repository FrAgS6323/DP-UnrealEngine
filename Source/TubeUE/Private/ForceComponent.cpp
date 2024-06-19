#include "ForceComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"

UForceComponent::UForceComponent(){
	PrimaryComponentTick.bCanEverTick = true;
	this->pidController = CreateDefaultSubobject<UPID>(TEXT("PIDComponent"));

	if (!this->pidController) {
		UE_LOG(LogTemp, Warning, TEXT("UPID not initialized!"));
	}
	else {
		this->pidController->setIdealPIDvalues();
		//this->pidController->setPIDvalues(this->P, this->I, this->D);
		this->pidController->setSaturationLimits(this->saturationMin, this->saturationMax);
	}
}

// Called when the game starts
void UForceComponent::BeginPlay(){
	Super::BeginPlay();
	this->meshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	for (TActorIterator<ATubeSolid> it(GetWorld()); it; ++it){
		this->tubeSolid = *it;
		break;
	}
}

double UForceComponent::getRegulationHeight(){
	return 45.0f;
}

void UForceComponent::PIDreg(double desiredHeight, bool onOff, float deltaTime) {
	if ((desiredHeight > this->getRegulationHeight()) ||
		(desiredHeight < 0))
		UE_LOG(LogTemp, Warning, TEXT("Wrong input! (Expected between 0 and %f)"), this->getRegulationHeight());

	if (!onOff) return;

	double senVal = this->tubeSolid->getDistance(),
		   error = UPID::estimateError(desiredHeight, senVal),
		   output = this->pidController ? this->pidController->getPIDOutput(error, (double)deltaTime) : 0;
	if(!this->pidController) UE_LOG(LogTemp, Warning, TEXT("UPID not initialized!"));

	UE_LOG(LogTemp, Warning, TEXT("PIDerror: = %f"), error);
	UE_LOG(LogTemp, Warning, TEXT("PIDoutput: = %f"), output);
	if (this->meshComp) this->meshComp->AddForce(FVector(0, 0, output));
}

// Called every frame
void UForceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	this->PIDreg(this->destinationHeight, this->PIDswitch, DeltaTime);
	//const FVector forceDirUp = this->GetUpVector();

	//UE_LOG(LogTemp, Warning, TEXT("Ball mass: %f"), this->meshComp->GetMass());

	//forceDirUp * this->force * this->meshComp->GetMass()
}