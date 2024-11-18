#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurtleLidarTop.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TUBEUE_API ATurtleLidarTop : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "Components") class UStaticMeshComponent* sBodyMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	ATurtleLidarTop();
	virtual void Tick(float DeltaTime) override;
};
