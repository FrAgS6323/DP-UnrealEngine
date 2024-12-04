#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotateWheel.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUBEUE_API URotateWheel : public UActorComponent{
	GENERATED_BODY()
	private:
		UStaticMeshComponent* sMeshComp;
		AActor* owner;
	protected:
		virtual void BeginPlay() override;
	public:	
		URotateWheel();
		//UPROPERTY(EditAnywhere, Category = "Velocity") double rotationVel;
		double rotationVel = 1000;
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};