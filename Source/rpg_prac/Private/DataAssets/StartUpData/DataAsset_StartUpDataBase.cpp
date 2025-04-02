

#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/CharacterGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UCharacterAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    check(InASCToGive);

    GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
    GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UCharacterGameplayAbility>>& InAbilitiesToGive, UCharacterAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    if (InAbilitiesToGive.IsEmpty())
    {
        return;
    }

    for (const TSubclassOf<UCharacterGameplayAbility>& Ability : InAbilitiesToGive)
    {
        if (!Ability) continue;

        FGameplayAbilitySpec AbilitySpec(Ability);
        AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;

        FGameplayAbilitySpecHandle Handle = InASCToGive->GiveAbility(AbilitySpec);

        // OnGiven 정책이면 바로 실행
        if (UCharacterGameplayAbility* AbilityCDO = Ability->GetDefaultObject<UCharacterGameplayAbility>())
        {
            if (AbilityCDO->GetActivationPolicy() == ECharacterAbilityActivationPolicy::OnGiven)
            {
                InASCToGive->TryActivateAbility(Handle);
            }
        }
    }
}
