
#pragma once

#include "NativeGameplayTags.h"

namespace RpgGameplayTags
{
	/** Input Tags **/
	RPG_PRAC_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	RPG_PRAC_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	RPG_PRAC_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
	RPG_PRAC_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);

	/** Player Tags **/
	RPG_PRAC_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);
	RPG_PRAC_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	RPG_PRAC_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);

}