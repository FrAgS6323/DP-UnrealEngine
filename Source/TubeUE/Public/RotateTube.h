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
		MeshType meshType;
		UStaticMeshComponent *meshComp;
	protected: 
		virtual void BeginPlay() override;
	public:	
		// Sets default values for this component's properties
		URotateTube();
		UPROPERTY(EditAnywhere, Category = "Angle") float angle = 0.0f;
		UPROPERTY(EditAnywhere, Category = "RotationVelocity") float rotationVel = 0.0f;
		enum class MeshType {SOLID, GLASS};
		void setMeshType(MeshType meshtype);
		//void rotate(float angleDeg);
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
