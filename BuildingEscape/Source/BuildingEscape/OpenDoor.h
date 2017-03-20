// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	
	//Working with Macros -jdeo
	UPROPERTY(VisibleAnywhere) //Make variable visible in details panel but not editable
	float OpenAngle = 90.0f;
	
	UPROPERTY(EditAnywhere) //Make variable visible in details panel and editable
	ATriggerVolume* PressurePlate;

	
	AActor* ActorThatOpens; //Remember pawn inherits from actor -jdeo
	
};
