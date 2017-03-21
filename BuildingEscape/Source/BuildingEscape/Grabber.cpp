// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

//Blank Macro -jdeo
#define OUT	

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// report for duty - jdeo
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Get player view point this tick -jdeo
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);
	// TODO: Log out to test -jdeo
	/*UE_LOG(LogTemp, Warning, 
		TEXT("Location: %s  Rotation: %s"),
		*PlayerViewPointLocation.ToString(),
		*PlayerViewPointRotation.ToString()
	);*/

	//Draw a red trace in the world to visualize -jdeo
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	DrawDebugLine(
		GetWorld(),					//
		PlayerViewPointLocation,	// LineStart
		LineTraceEnd,				// LineEnd
		FColor (255,0,0),			// Color of line
		false,						// Line persistence (false = redrawn every frame)
		0.0f,						// Lifetime
		0.0f,						// Depth Priority
		10.0f						// Line Thickness
	);

	// Ray-cast out to reach distance -jdeo

	// See what we hit -jdeo
}

