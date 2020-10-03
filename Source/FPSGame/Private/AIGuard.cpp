// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"


#include "ChaosInterfaceWrapperCore.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine/TargetPoint.h"

// Sets default values
AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnComponent"));

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIGuard::OnPawnSeen);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIGuard::OnHeardNoise);

	GuardState = EAIGuardState::Idle;
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorRotation();
	
}

void AAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if(SeenPawn == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Emerald, false, 10.f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		
	if(GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIGuardState::Alerted);

}

void AAIGuard::OnHeardNoise(APawn* AIInstigator, const FVector& Location, float Volume)
{

	if(GuardState == EAIGuardState::Alerted)
	{
		return;		
	}
	
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Red, false, 10.f);
	
	FVector Direction = Location - GetActorLocation();

	//same with FindLookAtRotation 
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AAIGuard::ResetOrientation, 3.0f);


	SetGuardState(EAIGuardState::Suspicious);
}


void AAIGuard::SetGuardState(const EAIGuardState& AIState)
{
	if(AIState == GuardState)
	{
		return;	
	}

	GuardState = AIState;

	OnStateChanged(GuardState);
}

void AAIGuard::MoveToNext()
{
	if(CurrentAIPatrolPoint == nullptr || CurrentAIPatrolPoint == SecondTargetPoint)
	{
		CurrentAIPatrolPoint = FirstTargetPoint;

	}
	else
	{
		CurrentAIPatrolPoint = SecondTargetPoint;
	}
	
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentAIPatrolPoint);
	
}

// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsPatrolEnable)
	{
		MoveToNext();
	}

}

// Called to bind functionality to input
void AAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIGuard::ResetOrientation()
{
	if(GuardState == EAIGuardState::Alerted)
	{
		return;		
	}
	
	SetActorRotation(InitialRotation);

	SetGuardState(EAIGuardState::Idle);
}
