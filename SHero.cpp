// Fill out your copyright notice in the Description page of Project Settings.


#include "SHero.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/App.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASHero::ASHero()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HeroCam = CreateDefaultSubobject<UCameraComponent>(TEXT("Hero_Camera"));
	HeroCam->SetupAttachment(GetCapsuleComponent());
	HeroCam->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));
	HeroCam->bUsePawnControlRotation = true;
	JumpBufferTime = 0.0f;

	


}

// Called when the game starts or when spawned
void ASHero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	JumpBufferTime -= DeltaTime;

	// Ensure JumpBufferTime doesn't go below 0
	JumpBufferTime = FMath::Max(0.0f, JumpBufferTime);

}

//Movement functions
void ASHero::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ASHero::MoveRight(float AxisValue)
{


	AddMovementInput(GetActorRightVector() * AxisValue);
	CameraTilt(AxisValue);
	
}


void ASHero::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ASHero::LookRight(float AxisValue)

{
	AddControllerYawInput(AxisValue);
	
}

void ASHero::CameraTilt(float AxisVal)
{
	AController* PlayerController = GetController();
	if (PlayerController)
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float Speed = 10.0f;
		FRotator PlayerControlRot = PlayerController->GetControlRotation();
		FRotator ControlROtOffset = FRotator(0.0f, 0.0f, AxisVal * 3);
		FRotator TiltRotation = FMath::RInterpTo(PlayerControlRot, ControlROtOffset, DeltaTime, Speed);
		//reset pitch and yaw of camera so you can actually turn to look
		TiltRotation.Pitch = PlayerControlRot.Pitch;
		TiltRotation.Yaw = PlayerControlRot.Yaw;
		PlayerController->SetControlRotation(TiltRotation);
	}
	
}
//all this for a stupid fucking bunny hop... started at 11:30 its not 4:10 am O.0
void ASHero::JumpFunction()
{
	if (CanJump())
	{
		Jump();
	}
	else
	{
		//if we spam space in air we have not landed set Bool Jump Buffer to true and countdown. 
		JumpBufferTime = 0.6f;	
	}

}

void ASHero::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	// Get the new movement mode
	EMovementMode NewMovementMode = GetCharacterMovement()->MovementMode;

	// was falling now landed
	if (PrevMovementMode == EMovementMode::MOVE_Falling && NewMovementMode == EMovementMode::MOVE_Walking)
	{
		//check buffer time is NOT 0
		if (JumpBufferTime > 0)
		{
			//Jump and reset
			JumpFunction();
			JumpBufferTime = 0.0f;
		}
	}
}



// Called to bind functionality to input
void ASHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASHero::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASHero::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ASHero::LookRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASHero::JumpFunction);

}

