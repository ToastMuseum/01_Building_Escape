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

	FindPhysicsHandleComponent();
	SetupInputComponent();
	

}



// Look for attached physics handle  -jdeo
void UGrabber::FindPhysicsHandleComponent() {
	// GetOwner object of Grabber.cpp. Then find physics component on the object
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		// Log an error message
		UE_LOG(LogTemp, Error, TEXT("%s is missing physics handle component"),
			*(GetOwner()->GetName())
		);
	}
}


// Look for attached Input Component (only appears at runtime) -jdeo
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		// Bind the input axis (grab and release) -jdeo
		InputComponent->BindAction(
			"Grab",					// Action Mapping Name
			IE_Pressed,				// Input Event
			this,					// Object the action is operated on
			&UGrabber::Grab			// Method Name on Object to be called
		);
		InputComponent->BindAction(
			"Grab",					// Action Mapping Name
			IE_Released,			// Input Event
			this,					// Object the action is operated on
			&UGrabber::Release		// Method Name on Object to be called
		);

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s: Input Component  Missing"),
			*(GetOwner()->GetName())
		);
	}
}



// LINE TRACE and see if we reach any actors with physics body collision channel set -jdeo
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// set up query parameters -jdeo
	FCollisionQueryParams TraceParameters(
		FName(TEXT("")),
		false,				// false uses simple collision (player collision view not visibility collision)
		GetOwner()			// Actor to ignore (trace starts in center of actor so ignore actor and allow it to pass through it)
	);

	/// line-trace (a.k.a. Ray-cast) out to reach distance -jdeo
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,					// hit result
		GetReachLineStart(),			// line start vector
		GetReachLineEnd(),				// line end vector
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),	// collision object query param
		TraceParameters					// Default query params
	);

	return HitResult;
}



void UGrabber::Grab() {

	/// LINE TRACE and see if we reach any actors with physics body collision channel set -jdeo
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();	// grabs the mesh component in our case -jdeo
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle -jdeo
	if (ActorHit) {
		// attach physics handle -jdeo
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,	
			NAME_None,			//no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}


void UGrabber::Release() {
	// release physics handle -jdeo
	PhysicsHandle->ReleaseComponent();
}




// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//TODO: if the physics handle is attached -jdeo
	if (PhysicsHandle->GrabbedComponent) {
		// move the object that we're holding  -jdeo
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

	//Draw red linetrace to show player reach -jdeo
	//DisplayDebugLineTrace();

}

FVector UGrabber::GetReachLineStart() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}


FVector UGrabber::GetReachLineEnd() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
}



// Show a line trace of where the player is looking -jdeo
void UGrabber::DisplayDebugLineTrace()
{
	// Get player view point this tick -jdeo
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);

	//Draw a red trace in the world to visualize -jdeo
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;


	DrawDebugLine(
		GetWorld(),					//
		PlayerViewPointLocation,	// LineStart
		LineTraceEnd,				// LineEnd
		FColor(255, 0, 0),			// Color of line
		false,						// Line persistence (false = redrawn every frame)
		0.0f,						// Lifetime
		0.0f,						// Depth Priority
		10.0f						// Line Thickness
	);

}