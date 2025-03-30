#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/DefaultPawn.h"
#include "TubeUEGameModeBase.generated.h"

UCLASS()
class TUBEUE_API ATubeUEGameModeBase : public AGameModeBase{
	GENERATED_BODY()
	public:
		ATubeUEGameModeBase();
};
