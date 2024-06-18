#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotateTube.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUBEUE_API URotateTube : public UActorComponent{
	GENERATED_BODY()
	
	private:
		FQuat initQuat;
		FQuat currentQuat;
		float currentAngleRad;

		bool isRotating;
		float targetAngleDeg;
		float rotationSpeed; // Degrees per second
	protected:
		virtual void BeginPlay() override;
	public:	
		// Sets default values for this component's properties
		URotateTube();
		void rotate(float angleDeg);
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
