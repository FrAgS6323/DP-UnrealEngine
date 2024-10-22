#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Tube.h"
#include "Holder.generated.h"

UCLASS(Blueprintable)
class TUBEUE_API AHolder : public AActor{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere) class UStaticMeshComponent* sHolderMesh;
	UPROPERTY(VisibleAnywhere) class ATube* tube;
	UPrimitiveComponent* tubeRootComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	AHolder();
	virtual void Tick(float DeltaTime) override;
};