

#include "AnimInstances/PlayerCharacterAnimInstance.h"
#include "Character/PlayerCharacter.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningPlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
}

void UPlayerCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElpasedTime = 0.f;
		bShouldRelaxState = false;
	}
	else
	{
		IdleElpasedTime += DeltaSeconds;

		bShouldRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
	}
}
