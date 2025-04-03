#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"
#include "Character/PlayerCharacter.h"
#include "PlayerController/RPG_PlayerController.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Weapon/WeaponBase.h"
#include <Abilities/Tasks/AbilityTask_WaitGameplayEvent.h>

UPlayerGameplayAbility::UPlayerGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

APlayerCharacter* UPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	if (!CachedPlayerCharacter.IsValid())
	{
		CachedPlayerCharacter = Cast<APlayerCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedPlayerCharacter.IsValid() ? CachedPlayerCharacter.Get() : nullptr;
}

ARPG_PlayerController* UPlayerGameplayAbility::GetRPG_PlayerControllerFromActorInfo()
{
	if (!CachedPlayerController.IsValid())
	{
		CachedPlayerController = Cast<ARPG_PlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedPlayerController.IsValid() ? CachedPlayerController.Get() : nullptr;
}

UPlayerCombatComponent* UPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

void UPlayerGameplayAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (!PlayerCharacter || !MontageToPlay)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitEquipEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Player.Event.Equip.Axe")),
		nullptr,
		true, false);

	if (WaitEquipEventTask)
	{
		WaitEquipEventTask->EventReceived.AddDynamic(this, &UPlayerGameplayAbility::OnEquipEventReceived);
		WaitEquipEventTask->ReadyForActivation();
	}

	USkeletalMeshComponent* Mesh = PlayerCharacter->GetMesh();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	if (AnimInstance)
	{
		const float Duration = AnimInstance->Montage_Play(MontageToPlay, 1.0f);
		if (Duration > 0.f)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UPlayerGameplayAbility::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
			return;
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UPlayerGameplayAbility::OnMontageEnded(UAnimMontage* Montage, bool bInterupted)
{
	if (Montage == MontageToPlay)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bInterupted);
	}
}

void UPlayerGameplayAbility::OnEquipEventReceived(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT(" Equip 이벤트 수신 완료!"));

	// 무기 컴포넌트 가져오기
	UPlayerCombatComponent* CombatComponent = GetPlayerCombatComponentFromActorInfo();
	if (!CombatComponent)
	{
		UE_LOG(LogTemp, Error, TEXT(" CombatComponent 없음"));
		return;
	}

	// 무기 태그로 기존 무기 가져오기
	const FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag(FName("Player.Weapon.Axe"));
	AWeaponBase* Weapon = CombatComponent->GetCharacterCarriedWeaponByTag(WeaponTag);
	if (!Weapon)
	{
		UE_LOG(LogTemp, Error, TEXT(" 등록된 무기 없음"));
		return;
	}

	// 무기 메시 가져오기
	USkeletalMeshComponent* Mesh = GetPlayerCharacterFromActorInfo()->GetMesh();
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT(" 메시 없음"));
		return;
	}

	// 등에서 떼고 손에 붙이기
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
	Weapon->AttachToComponent(Mesh, AttachRules, FName("RightHandSocket"));

	UE_LOG(LogTemp, Warning, TEXT("무기 손에 장착 성공"));
}
