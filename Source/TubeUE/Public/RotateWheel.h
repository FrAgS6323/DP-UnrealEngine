#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotateWheel.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUBEUE_API URotateWheel : public UActorComponent{
	GENERATED_BODY()
	private:
		UStaticMeshComponent* sMeshComp;
	protected:
		virtual void BeginPlay() override;
	public:	
		URotateWheel();
		UPROPERTY(EditAnywhere, Category = "Torque") double rotationTorque;
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
