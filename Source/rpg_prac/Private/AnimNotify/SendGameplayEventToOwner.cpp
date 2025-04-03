

#include "AnimNotify/SendGameplayEventToOwner.h"
#include "AbilitySystemBlueprintLibrary.h"

void USendGameplayEventToOwner::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, FGameplayEventData());

}
