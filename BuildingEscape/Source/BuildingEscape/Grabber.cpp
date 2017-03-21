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

	/// set up query parameters -jdeo
	FCollisionQueryParams TraceParameters(
		FName(TEXT("")),	
		false,				// false uses simple collision (player collision view not visibility collision)
		GetOwner()			// Actor to ignore (trace starts in center of actor so ignore actor and allow it to pass through it)
	);

	/// line-trace (a.k.a. Ray-cast) out to reach distance -jdeo
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,					// hit result
		PlayerViewPointLocation,	// line start vector
		LineTraceEnd,				// line end vector
		FCollisionObjectQueryParams	(ECollisionChannel::ECC_PhysicsBody),	// collision object query param
		TraceParameters				// Default query params
	);

	/// See what we hit -jdeo
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning,
			TEXT("LineTrace Hit: %s"),
			*(ActorHit->GetName())
		);
	}

}

