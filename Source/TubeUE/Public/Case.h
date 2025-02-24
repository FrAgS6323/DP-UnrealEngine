#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "Case.generated.h"

UCLASS()
class TUBEUE_API ACase : public AActor
{
	GENERATED_BODY()
	private:
		TObjectPtr<UStaticMeshComponent> sCaseMesh;
		TObjectPtr < UMaterialInterface> mRedButton;
		//TObjectPtr<UPointLightComponent> pointLight;
	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	public:	
		// Sets default values for this actor's properties
		ACase();
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bBlueButton;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bRedButton;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bGreenButton;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons") bool bYellowButton;
		// Called every frame
		virtual void Tick(float DeltaTime) override;
};