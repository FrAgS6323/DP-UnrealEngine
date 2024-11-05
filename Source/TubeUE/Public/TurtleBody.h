#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurtleBody.generated.h"

UCLASS(Blueprintable)
class TUBEUE_API ATurtleBody : public AActor
{
	GENERATED_BODY()
	private:
		UPROPERTY(VisibleAnywhere) class UStaticMeshComponent* sBodyMesh;
	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	public:	
		ATurtleBody();
		virtual void Tick(float DeltaTime) override;
};
