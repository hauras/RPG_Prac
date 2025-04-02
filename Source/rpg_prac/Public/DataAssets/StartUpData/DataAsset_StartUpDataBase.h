
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UCharacterGameplayAbility;
class UCharacterAbilitySystemComponent;

UCLASS()
class RPG_PRAC_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
public:
    virtual void GiveToAbilitySystemComponent(UCharacterAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
    TArray< TSubclassOf < UCharacterGameplayAbility > > ActivateOnGivenAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
    TArray< TSubclassOf < UCharacterGameplayAbility > > ReactiveAbilities;

    void GrantAbilities(const TArray< TSubclassOf < UCharacterGameplayAbility > >& InAbilitiesToGive, UCharacterAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
