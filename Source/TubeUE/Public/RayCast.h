#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RayCast.generated.h"

UCLASS()
class TUBEUE_API ARayCast : public AActor{
	GENERATED_BODY()
	private:
		UPROPERTY(EditAnywhere, Category = "RayCast") FVector rayStart;

		// Direction and length of the RayCast
		UPROPERTY(EditAnywhere, Category = "RayCast") FVector rayDirection;

		// Distance of the RayCast
		UPROPERTY(EditAnywhere, Category = "RayCast") float rayLength = 1000.f;
	protected:
		virtual void BeginPlay() override;
	public:	
		ARayCast();
		void performRayCast();
		virtual void Tick(float DeltaTime) override;
};
