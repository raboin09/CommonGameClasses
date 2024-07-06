#pragma once

#include "ResourceComponent.h"
#include "Types/CommonEventDeclarations.h"
#include "EnergyComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMMONGAMECLASSES_API UEnergyComponent : public UResourceComponent
{
	GENERATED_BODY()

public:	
	UEnergyComponent();
	
	///////////////////////////////////
	/// IResourceContainer overrides
	////////////////////////////////////
	virtual bool TryConsumeResourceAmount(const float RequestedAmount) override;
	virtual void TryGiveResourceAmount(const float AmountToGive) override;

	///////////////////////////////////
	/// ResourceComponent overrides
	////////////////////////////////////
	virtual float CalculateConsumptionAmount(const float RequestedAmount) const override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	void BroadcastEnergyChangedEvent() const;
	
	UPROPERTY(BlueprintAssignable)
	FEnergyAmountChangedEvent EnergyAmountChanged;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON")
	FORCEINLINE float GetCurrentEnergy() const { return GetAvailableResourceAmount(); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "COMMON")
	FORCEINLINE float GetMaxEnergy() const { return GetMaxResourceAmount(); }
	FORCEINLINE virtual FEnergyAmountChangedEvent& OnEnergyChanged() { return EnergyAmountChanged; }
};