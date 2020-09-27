// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

class UStaticMesh;
class UBoxComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Category")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Category")
	UBoxComponent* BoxComponent;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	float LaunchStrength;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	float LaunchPitchAngle;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
	UParticleSystem* ActivateLaunchPadEffect;
	
};
