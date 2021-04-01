// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class UFightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CPP_GAME_API IFightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	#pragma region FIGHT_INTERFACE_FUNTION
		UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category = Attacks)
		void CheckPunch(bool isLeftHand);
	
		UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category = Attacks)
		void CheckKick(bool isLeftLeg);
	#pragma endregion 	
};
