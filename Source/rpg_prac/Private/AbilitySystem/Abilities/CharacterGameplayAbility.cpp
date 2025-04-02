#include "AbilitySystem/Abilities/CharacterGameplayAbility.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "Weapon/WeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

void UCharacterGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == ECharacterAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UCharacterGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!WeaponClassToSpawn || !ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();
	FRotator SpawnRotation = ActorInfo->AvatarActor->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = ActorInfo->AvatarActor.Get();
	SpawnParams.Instigator = Cast<APawn>(ActorInfo->AvatarActor.Get());

	UWorld* World = ActorInfo->AvatarActor->GetWorld();
	if (World)
	{
		SpawnedWeapon = World->SpawnActor<AWeaponBase>(WeaponClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedWeapon)
		{
			AActor* Avatar = ActorInfo->AvatarActor.Get();
			if (USkeletalMeshComponent* Mesh = Avatar->FindComponentByClass<USkeletalMeshComponent>())
			{
				FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
				SpawnedWeapon->AttachToComponent(Mesh, AttachRules, WeaponAttachSocketName);

				// 필요하다면 초기화 로직 추가 가능
			}
		}
	}
}

void UCharacterGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == ECharacterAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}

	if (SpawnedWeapon)
	{
		SpawnedWeapon->Destroy();
		SpawnedWeapon = nullptr;
	}
}
