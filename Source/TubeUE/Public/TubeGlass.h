#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotateTube.h"
#include "TubeGlass.generated.h"

UCLASS()
class TUBEUE_API ATubeGlass : public AActor{
	GENERATED_BODY()
	private:
		double motorHeight = 0.07f;
		UPROPERTY(EditAnywhere) class UStaticMeshComponent* tubeGlassMesh;
	protected:
		virtual void BeginPlay() override;
	public:
		ATubeGlass();
		UPROPERTY(EditAnywhere) float angle;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") URotateTube* rotateTubeComponent;
		virtual void Tick(float DeltaTime) override;
};