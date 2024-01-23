// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SHero.generated.h"


class UCameraComponent;
class UCapsuleComponent;

UCLASS()
class ZOMBIEFPS_API ASHero : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASHero();


	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UFUNCTION(BlueprintCallable)
	void MoveForward(float AxisValue);

	//UFUNCTION(BlueprintCallable)
	void MoveRight(float AxisValue);

	UFUNCTION(BlueprintCallable)
	void LookUp(float AxisValue);

	UFUNCTION(BlueprintCallable)
	void LookRight(float AxisValue);

	UFUNCTION(BlueprintCallable)
	void JumpFunction();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* HeroCam;

	UFUNCTION()
	void CameraTilt(float AxisVal);

	float JumpBufferTime;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
