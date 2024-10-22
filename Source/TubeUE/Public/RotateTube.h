#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotateTube.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUBEUE_API URotateTube : public UActorComponent{
	GENERATED_BODY()
	private:
#if 0
		FQuat initQuat;
		FQuat currentQuat;
		float currentAngleRad;

		bool isRotating;
		float targetAngleDeg;
		float rotationSpeed; // Degrees per second
#endif
		UStaticMeshComponent *meshComp;
	protected: 
		virtual void BeginPlay() override;
	public:	
		// Sets default values for this component's properties
		URotateTube();
		UPROPERTY(EditAnywhere, Category = "Angle") double angle = 0.0;
		UPROPERTY(EditAnywhere, Category = "Torque")double rotationTorque = 0.0;
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
