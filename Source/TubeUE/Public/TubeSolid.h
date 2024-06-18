#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotateTube.h"
#include "TubeSolid.generated.h"

UCLASS()
class TUBEUE_API ATubeSolid : public AActor{
	
	GENERATED_BODY()
	private:
		UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* tubeSolidMesh;
	protected:
		virtual void BeginPlay() override;
	public:	
		// Sets default values for this actor's properties
		ATubeSolid();
		UPROPERTY(EditAnywhere)
		float angle;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		URotateTube* rotateTubeComponent;
		virtual void Tick(float DeltaTime) override;
};
