
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CharacterGameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

class APlayerCharacter;
class ARPG_PlayerController;
class UPlayerCombatComponent;
class UAbilityTask_WaitGameplayEvent;

UCLASS()
class RPG_PRAC_API UPlayerGameplayAbility : public UCharacterGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPlayerGameplayAbility(); // 생성자 선언 추가

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	APlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	ARPG_PlayerController* GetRPG_PlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	TWeakObjectPtr<APlayerCharacter> CachedPlayerCharacter;
	TWeakObjectPtr<ARPG_PlayerController> CachedPlayerController;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Montage")
	UAnimMontage* MontageToPlay;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterupted);

	UFUNCTION()
	void OnEquipEventReceived(FGameplayEventData Payload);

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* WaitEquipEventTask;
};
