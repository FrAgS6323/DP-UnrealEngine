#pragma once
#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Components/ActorComponent.h"
#include "TurtleBotTypeEnum.h"
#include "RotateWheelRight.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TUBEUE_API URotateWheelRight : public UActorComponent {
	GENERATED_BODY()
	private:
		int ticksPassed = 0,
			   idx = 0,
			   actualSeriesOfTicks = 0;
		static constexpr double rotationVelForward = 1000;
		static constexpr double rotationVelStop = 1;
		UStaticMeshComponent* sMeshComp;
		AActor* owner;
	protected:
		virtual void BeginPlay() override;
	public:
		URotateWheelRight();
		//UPROPERTY(EditAnywhere, Category = "Velocity") double rotationVel;
		double rotationVel;
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	};