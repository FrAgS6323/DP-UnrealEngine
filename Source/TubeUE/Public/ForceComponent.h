#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PID.h"
#include "TubeSolid.h"
#include "ForceComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUBEUE_API UForceComponent : public USceneComponent{
	GENERATED_BODY()
	private:
		UPROPERTY(EditAnywhere, Category = "PID Values") bool PIDswitch = false;
		UPROPERTY(EditAnywhere, Category = "PID Values") double destinationHeight = 20.0;
		UPROPERTY(EditAnywhere, Category = "PID Values") double P = UPID::idealP;
		UPROPERTY(EditAnywhere, Category = "PID Values") double I = UPID::idealI;
		UPROPERTY(EditAnywhere, Category = "PID Values") double D = UPID::idealD;
		UPROPERTY(EditAnywhere, Category = "PID Values") double saturationMin = 1.5;
		UPROPERTY(EditAnywhere, Category = "PID Values") double saturationMax = 5.0;
		UPROPERTY() UStaticMeshComponent* meshComp;
		UPID* pidController;
		ATubeSolid *tubeSolid;
		double getRegulationHeight();
		void PIDreg(double desiredHeight, 
					bool onOff, 
					float deltaTime);
	protected:
		virtual void BeginPlay() override;
	public:
		UForceComponent();
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};