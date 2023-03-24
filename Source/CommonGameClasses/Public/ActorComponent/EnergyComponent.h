#pragma once

#include "ResourceComponent.h"
#include "Types/EventDeclarations.h"
#include "EnergyComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMMONGAMECLASSES_API UEnergyComponent : public UResourceComponent
{
	GENERATED_BODY()

public:	
	UEnergyComponent();
	virtual bool TrySpendResource(const float RequestedAmount) override;
	virtual void GiveResource(const float AmountToGive) override;
	virtual float CalculateResourceCost(const float RequestedAmount) const override;

protected:
	virtual void BeginPlay() override;
	
private:
	void BroadcastEnergyChangedEvent() const;
	
	FEnergyAmountChangedEvent EnergyAmountChanged;

public:
	FORCEINLINE virtual FEnergyAmountChangedEvent& OnEnergyChanged() { return EnergyAmountChanged; }
};