#pragma once

#include "CoreMinimal.h"
#include "GameFinishResult.generated.h"

UENUM(BlueprintType)
enum class EGameFinishResult : uint8
{
	Cleared,
	Failed
};
