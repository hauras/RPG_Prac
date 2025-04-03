

#include "AbilitySystem/CharacterAbilitySystemComponent.h"

void UCharacterAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
	}

}

void UCharacterAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{

}
