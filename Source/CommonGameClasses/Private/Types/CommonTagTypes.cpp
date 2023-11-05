#include "Types/CommonTagTypes.h"

namespace CommonGameSlot
{
	UE_DEFINE_GAMEPLAY_TAG(SlotMain, FName{TEXTVIEW("Slot.Main")})	
}

namespace CommonGameActorType
{
	UE_DEFINE_GAMEPLAY_TAG(ActorTypeCover, FName{TEXTVIEW("ActorType.Cover")})	
}

namespace CommonGameAbilityEvent
{
	UE_DEFINE_GAMEPLAY_TAG(OnCooldown, FName{TEXTVIEW("AbilityEvent.OnCooldown")})
	UE_DEFINE_GAMEPLAY_TAG(Activated, FName{TEXTVIEW("AbilityEvent.Activated")})
	UE_DEFINE_GAMEPLAY_TAG(RequestingStart, FName{TEXTVIEW("AbilityEvent.RequestingStart")})
	UE_DEFINE_GAMEPLAY_TAG(Committed, FName{TEXTVIEW("AbilityEvent.Committed")})
	UE_DEFINE_GAMEPLAY_TAG(ComboWindowEnabled, FName{TEXTVIEW("AbilityEvent.ComboWindowEnabled")})
	UE_DEFINE_GAMEPLAY_TAG(ComboActivated, FName{TEXTVIEW("AbilityEvent.ComboActivated")})
	UE_DEFINE_GAMEPLAY_TAG(Active, FName{TEXTVIEW("AbilityEvent.Active")})	
}

namespace CommonGameState
{
	UE_DEFINE_GAMEPLAY_TAG(Active, FName{TEXTVIEW("State.Active")})
	UE_DEFINE_GAMEPLAY_TAG(Aiming, FName{TEXTVIEW("State.Aiming")})
	UE_DEFINE_GAMEPLAY_TAG(Stunned, FName{TEXTVIEW("State.Stunned")})
	UE_DEFINE_GAMEPLAY_TAG(Immovable, FName{TEXTVIEW("State.Immovable")})
	UE_DEFINE_GAMEPLAY_TAG(CannotSprint, FName{TEXTVIEW("State.CannotSprint")})
	UE_DEFINE_GAMEPLAY_TAG(Aloof, FName{TEXTVIEW("State.Aloof")})
	UE_DEFINE_GAMEPLAY_TAG(Loading, FName{TEXTVIEW("State.Loading")})
	UE_DEFINE_GAMEPLAY_TAG(ArmorPiercing, FName{TEXTVIEW("State.ArmorPiercing")})
	UE_DEFINE_GAMEPLAY_TAG(Dead, FName{TEXTVIEW("State.Dead")})
	UE_DEFINE_GAMEPLAY_TAG(Firing, FName{TEXTVIEW("State.Firing")})
	UE_DEFINE_GAMEPLAY_TAG(Ready, FName{TEXTVIEW("State.Ready")})
	UE_DEFINE_GAMEPLAY_TAG(Ragdoll, FName{TEXTVIEW("State.Ragdoll")})
	UE_DEFINE_GAMEPLAY_TAG(InCover, FName{TEXTVIEW("State.InCover")})
	UE_DEFINE_GAMEPLAY_TAG(CannotMount, FName{TEXTVIEW("State.CannotMount")})
	UE_DEFINE_GAMEPLAY_TAG(InCombat, FName{TEXTVIEW("State.InCombat")})	
	UE_DEFINE_GAMEPLAY_TAG(CoverLeftPeek, FName{TEXTVIEW("State.InCover.LeftPeek")})
	UE_DEFINE_GAMEPLAY_TAG(CoverLeftEdge, FName{TEXTVIEW("State.InCover.LeftEdge")})
	UE_DEFINE_GAMEPLAY_TAG(CoverRightPeek, FName{TEXTVIEW("State.InCover.RightPeek")})
	UE_DEFINE_GAMEPLAY_TAG(CoverRightEdge, FName{TEXTVIEW("State.InCover.RightEdge")})
	UE_DEFINE_GAMEPLAY_TAG(CoverRolledOut, FName{TEXTVIEW("State.InCover.RolledOut")})
}

namespace CommonGameAnimation
{
	UE_DEFINE_GAMEPLAY_TAG(HitReactLeft, FName{TEXTVIEW("Animation.HitReact.Left")})
	UE_DEFINE_GAMEPLAY_TAG(HitReactFront, FName{TEXTVIEW("Animation.HitReact.Front")})
	UE_DEFINE_GAMEPLAY_TAG(HitReactRight, FName{TEXTVIEW("Animation.HitReact.Right")})
	UE_DEFINE_GAMEPLAY_TAG(HitReactBack, FName{TEXTVIEW("Animation.HitReact.Back")})
	UE_DEFINE_GAMEPLAY_TAG(HitReactChainsaw, FName{TEXTVIEW("Animation.HitReact.Chainsaw")})
	UE_DEFINE_GAMEPLAY_TAG(Unarmed, FName{TEXTVIEW("Animation.Overlay.Unarmed")})
}
