// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_EquipInterp UMETA(DisplayName = "EquipInterp"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),
	
	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DarkColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBackground;
};

UCLASS()
class SHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void SetActiveStars();

	virtual void SetItemProperties(EItemState State);

	void FinishInterping();

	/** Handles Item interpolation when in interp state*/
	void ItemInterp(float DeltaTime);

	virtual void InitializeCustomDepth();

	virtual void OnConstruction(const FTransform& Transform) override;

	void EnableGlowMaterial();

	void ResetPulseTimer();

	void StartPulseTimer();

	void UpdatePulse();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayEquipSound(bool bForcePlaySound = false);

private:
	/** Skeletal Mesh for the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;

	/** Line trace collides with box to show HUD widgets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	/** Ammo count*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector ItemInterpStartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector CameraTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bInterping;

	FTimerHandle ItemInterpTimer;

	/** Duration of the curve & timer*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float ZCurveTime;

	/** Pointer to the character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* Character;

	float ItemInterpX;
	float ItemInterpY;

	/** Initial offset between camera and item*/
	float InterpInitialYawOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemScaleCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* EquipSound;

	/** Index for the material we'd like to change at runtime*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 MaterialIndex;

	/** Dynamic Instance that we can change at runtime*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	/** Material instance used with Dynamic Instance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* MaterialInstance;

	bool bCanChangeCustomDepth;

	/** Curve to drive the dynamic material parameter */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveVector* PulseCurve;

	FTimerHandle PulseTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float PulseCurveTime;

	UPROPERTY(VisibleAnywhere,  Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float GlowAmount;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float FresnelExponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float FresnelReflectFraction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UTexture2D* AmmoIcon;

	/** Slot in the inventory array */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	bool bInventoryFull;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemRarityDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = "true"))
	FLinearColor GlowColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = "true"))
	FLinearColor LightColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = "true"))
	FLinearColor DarkColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = "true"))
	int32 NumberOfStars;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rarity", meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconBackground;

public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }

	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }

	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }

	FORCEINLINE EItemState GetItemState() const { return ItemState; }

	void SetItemState(EItemState State);

	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }

	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound; }

	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }

	FORCEINLINE void SetPickupSound(USoundCue* Sound) { PickupSound = Sound; }

	FORCEINLINE void SetEquipSound(USoundCue* Sound) { EquipSound = Sound; }

	FORCEINLINE void SetItemName(FString Name) { ItemName = Name; }

	FORCEINLINE int32 GetItemCount() const { return ItemCount; }

	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }

	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex = Index; }

	void StartItemCurve(AShooterCharacter* Char);

	virtual void EnableCustomDepth();

	virtual void DisableCustomDepth();

	void DisableGlowMaterial();

	FORCEINLINE void SetCharacter(AShooterCharacter* Char) { Character = Char; }

	FORCEINLINE void SetInventoryFull(bool bFull) { bInventoryFull = bFull; }

	FORCEINLINE void SetIconItem(UTexture2D* Icon) { ItemIcon = Icon; }

	FORCEINLINE void SetAmmoIcon(UTexture2D* Icon) { AmmoIcon = Icon; }

	FORCEINLINE UMaterialInstance* GetMaterialInstance() const { return MaterialInstance; }

	FORCEINLINE void SetMaterialInstance(UMaterialInstance* Instance) { MaterialInstance = Instance; }

	FORCEINLINE UMaterialInstanceDynamic* GetDynamicMaterialInstance() const { return DynamicMaterialInstance; }

	FORCEINLINE void SetDynamicMaterialInstance(UMaterialInstanceDynamic* Instance) { DynamicMaterialInstance = Instance; }

	FORCEINLINE FLinearColor GetGlowColor() const { return GlowColor; }

	FORCEINLINE int32 GetMaterialIndex() const { return MaterialIndex; }

	FORCEINLINE void SetMaterialIndex(int32 Index) { MaterialIndex = Index; }

};
