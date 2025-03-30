
#include "Character/PlayerCharacter.h"

#include "RpgDebugHelper.h"

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("Working"));
}
