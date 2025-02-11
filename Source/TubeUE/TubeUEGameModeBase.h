#pragma once

#include "CoreMinimal.h"
#include "TurtleBotPawn.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/DefaultPawn.h"
#include "TubeUEGameModeBase.generated.h"

UCLASS()
class TUBEUE_API ATubeUEGameModeBase : public AGameModeBase{
	GENERATED_BODY()

	private:
		UPROPERTY(EditAnywhere, NoClear)
		TSubclassOf<ATurtleBody> turtlePawn;
	public:
		void InitGameState() override;
};
