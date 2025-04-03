#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterGameplayAbility.generated.h"

class AWeaponBase;
class UPawnCombatComponent;
class UCharacterAbilitySystemComponent;
UENUM(BlueprintType)
enum class ECharacterAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

UCLASS()
class RPG_PRAC_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	ECharacterAbilityActivationPolicy GetActivationPolicy() const { return AbilityActivationPolicy; }

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterAbility")
	ECharacterAbilityActivationPolicy AbilityActivationPolicy = ECharacterAbilityActivationPolicy::OnTriggered;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeaponBase> WeaponClassToSpawn;

	UPROPERTY()
	AWeaponBase* SpawnedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponAttachSocketName = "AxeBackSocket";

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponentFromActionInfo() const;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Register")
	FGameplayTag WeaponTagToRegister;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Register")
	bool bRegisterAsEquippedWeapon = false;
};
