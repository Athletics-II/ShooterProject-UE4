#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_SMG UMETA(DisplayName = "SubmachineGun"),
	EWT_AR UMETA(DisplayName = "AssaultRifle"),

	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};
