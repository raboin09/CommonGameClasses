  #include "Types/CommonTagTypes.h"

namespace CommonGameSlot
{
	UE_DEFINE_GAMEPLAY_TAG(SlotMain, FName{TEXTVIEW("Slot.Main")})
	UE_DEFINE_GAMEPLAY_TAG(SlotAlt, FName{TEXTVIEW("Slot.Alt")})
	UE_DEFINE_GAMEPLAY_TAG(SlotJump, FName{TEXTVIEW("Slot.Jump")})	
}

namespace CommonGameAbilityEvent
{
	UE_DEFINE_GAMEPLAY_TAG(StartAbilityAfterEquip, FName{TEXTVIEW("AbilityEvent.StartAbilityAfterEquip")})
	UE_DEFINE_GAMEPLAY_TAG(Equipping, FName{TEXTVIEW("AbilityEvent.Equipping")})
	UE_DEFINE_GAMEPLAY_TAG(OnCooldown, FName{TEXTVIEW("AbilityEvent.OnCooldown")})
	UE_DEFINE_GAMEPLAY_TAG(Activated, FName{TEXTVIEW("AbilityEvent.Activated")})
	UE_DEFINE_GAMEPLAY_TAG(RequestingStart, FName{TEXTVIEW("AbilityEvent.RequestingStart")})
	UE_DEFINE_GAMEPLAY_TAG(Committed, FName{TEXTVIEW("AbilityEvent.Committed")})
	UE_DEFINE_GAMEPLAY_TAG(ComboWindowEnabled, FName{TEXTVIEW("AbilityEvent.ComboWindowEnabled")})
	UE_DEFINE_GAMEPLAY_TAG(ComboActivated, FName{TEXTVIEW("AbilityEvent.ComboActivated")})
	UE_DEFINE_GAMEPLAY_TAG(Active, FName{TEXTVIEW("AbilityEvent.Active")})
	UE_DEFINE_GAMEPLAY_TAG(AutoStartAbility, FName{TEXTVIEW("AbilityEvent.AutoRestartAbility")})
}

namespace CommonGameState
{
	UE_DEFINE_GAMEPLAY_TAG(Active, FName{TEXTVIEW("State.Active")})
	UE_DEFINE_GAMEPLAY_TAG(Aiming, FName{TEXTVIEW("State.Aiming")})
	UE_DEFINE_GAMEPLAY_TAG(Stunned, FName{TEXTVIEW("State.Stunned")})
	UE_DEFINE_GAMEPLAY_TAG(Immovable, FName{TEXTVIEW("State.Immovable")})
	UE_DEFINE_GAMEPLAY_TAG(Loading, FName{TEXTVIEW("State.Loading")})
	UE_DEFINE_GAMEPLAY_TAG(Dashing, FName{TEXTVIEW("State.Dashing")})
	UE_DEFINE_GAMEPLAY_TAG(Dead, FName{TEXTVIEW("State.Dead")})
	UE_DEFINE_GAMEPLAY_TAG(Firing, FName{TEXTVIEW("State.Firing")})
	UE_DEFINE_GAMEPLAY_TAG(Sprinting, FName{TEXTVIEW("State.Sprinting")})
	UE_DEFINE_GAMEPLAY_TAG(Ready, FName{TEXTVIEW("State.Ready")})
	UE_DEFINE_GAMEPLAY_TAG(Ragdoll, FName{TEXTVIEW("State.Ragdoll")})
	UE_DEFINE_GAMEPLAY_TAG(CannotMount, FName{TEXTVIEW("State.CannotMount")})
	UE_DEFINE_GAMEPLAY_TAG(MeleeEquipped, FName{TEXTVIEW("State.MeleeEquipped")})
}

namespace CommonGameAnimation
{
	UE_DEFINE_GAMEPLAY_TAG(Unarmed, FName{TEXTVIEW("Animation.Overlay.Unarmed")})
	
	UE_DEFINE_GAMEPLAY_TAG(HitReactLeft, FName{TEXTVIEW("Animation.HitReact.Left")})
	UE_DEFINE_GAMEPLAY_TAG(HitReactFront, FName{TEXTVIEW("Animation.HitReact.Front")})
	UE_DEFINE_GAMEPLAY_TAG(HitReactRight, FName{TEXTVIEW("Animation.HitReact.Right")})
	UE_DEFINE_GAMEPLAY_TAG(HitReactBack, FName{TEXTVIEW("Animation.HitReact.Back")})
	UE_DEFINE_GAMEPLAY_TAG(HitReactSpecial1, FName{TEXTVIEW("Animation.HitReact.Special1")})
}
