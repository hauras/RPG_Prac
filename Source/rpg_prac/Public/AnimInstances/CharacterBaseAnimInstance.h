
#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/RpgBaseAnimInstance.h"
#include "CharacterBaseAnimInstance.generated.h"

class ACharacterBase;
class UCharacterMovementComponent;
UCLASS()
class RPG_PRAC_API UCharacterBaseAnimInstance : public URpgBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

protected:

	UPROPERTY()
	ACharacterBase* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

};
