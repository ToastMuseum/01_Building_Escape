// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Get Owner Actor for door -jdeo
	Owner = GetOwner();

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s Missing pressure plate"),
			*(GetOwner()->GetName())
		);
	}

}

void UOpenDoor::OpenDoor()
{
	if (!Owner) { return; }
	// Set the door rotation -jdeo
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	if (!Owner) { return; }
	// Set the door rotation -jdeo
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume -jdeo
	// If mass on plate exceeds maxWeight open door -jdeo
	if (GetTotalMassOfActorsOnPlate() > MaxWeight){
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if its time to close the door
	if (GetWorld()->GetTimeSeconds() > LastDoorOpenTime + DoorCloseDelay) {
		CloseDoor();
	}
	
	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {

	float TotalMass = 0.0f;

	if (!PressurePlate) {return TotalMass;}
	// find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(
		OUT OverlappingActors
	);

	// iterate through actors adding their mass
	for (const auto* Actor : OverlappingActors) {

		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"),
			*(Actor->GetName())
		);

		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	return TotalMass;
}