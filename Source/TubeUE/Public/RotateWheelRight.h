#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotateWheelRight.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TUBEUE_API URotateWheelRight : public UActorComponent {
	GENERATED_BODY()
private:
	UStaticMeshComponent* sMeshComp;
	AActor* owner;
protected:
	virtual void BeginPlay() override;
public:
	URotateWheelRight();
	//UPROPERTY(EditAnywhere, Category = "Velocity") double rotationVel;
	double rotationVel = 1000;
	size_t count = 0;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};