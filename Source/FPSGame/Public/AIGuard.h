// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AIGuard.generated.h"

UENUM(BlueprintType)
enum class EAIGuardState : uint8
{
	Idle,
	Suspicious,
	Alerted

};

class UPawnSensingComponent;

UCLASS()
class FPSGAME_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "AISettings")
	bool bIsPatrolEnable = false;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
	
	UFUNCTION()
	void OnHeardNoise( APawn* AIInstigator, const FVector& Location, float Volume);

	FRotator InitialRotation;

	UFUNCTION()
	void ResetOrientation();

	FTimerHandle TimerHandle_ResetOrientation;

	UPROPERTY(EditInstanceOnly, Category = "AISettings", meta = (EditCondition = "bIsPatrolEnable"))
	AActor* FirstTargetPoint;

	UPROPERTY(EditInstanceOnly, Category = "AISettings", meta = (EditCondition = "bIsPatrolEnable"))
	AActor* SecondTargetPoint;
	
	AActor* CurrentAIPatrolPoint;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
    void OnStateChanged(EAIGuardState AIState);
	
	EAIGuardState GuardState;

	void SetGuardState(const EAIGuardState& AIState);

	void MoveToNextPatrolPoint();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
