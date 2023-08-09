// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "UObject/Interface.h"
#include "AB_Pawn_To_SoundCube_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAB_Pawn_To_SoundCube_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ANONYMOUSBAT_2_API IAB_Pawn_To_SoundCube_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void MusicStart();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void SoundCubeVisualizer_MouseButtonDown(UPrimitiveComponent* _ClosestHit);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void SoundCubeVisualizer_MouseButtonUp(UPrimitiveComponent* _ClosestHit);
};
