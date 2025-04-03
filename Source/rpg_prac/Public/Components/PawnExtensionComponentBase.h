
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_PRAC_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()


protected:
	template<class T> 
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template parameter to GetPawn");
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template <class T>
	T* GetOwningComtroller() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template parameter to GetController");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
};
