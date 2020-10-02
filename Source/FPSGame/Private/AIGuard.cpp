// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"


#include "ChaosInterfaceWrapperCore.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnComponent"));

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIGuard::OnPawnSeen);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIGuard::OnHeardNoise);
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
}

void AAIGuard::OnHeardNoise(APawn* AIInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Red, false, 10.f);
	
	FVector Direction = Location - GetActorLocation();

	//same with FindLookAtRotation 
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AAIGuard::ResetOrientation, 3.0f);
}



// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIGuard::ResetOrientation()
{
	SetActorRotation(InitialRotation);
}
