// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Character.h"

#include "Navigation/PathFollowingComponent.h"

// Sets default values
ACPP_Character::ACPP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Character_Doesnt_Rotate_With_Camera

		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw =  false;

	#pragma endregion

		GetCharacterMovement()->bOrientRotationToMovement = false; //Character rotate to move direction

	#pragma region AddSpringArm

		CameraBoom = CreateDefaultSubobject <USpringArmComponent>(TEXT("CameraBoom"));
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->SetRelativeRotation(FRotator(0,-90,0));
		CameraBoom->TargetArmLength=500;
		//CameraBoom->bUsePawnControlRotation = true; //camera move with controller
		
	#pragma endregion

	#pragma region AddCameraToSpringArm

		PlayerCamera = CreateAbstractDefaultSubobject <UCameraComponent>(TEXT("PlayerCamera"));
		PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	#pragma endregion


}

// Called when the game starts or when spawned
void ACPP_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	#pragma region Turn_Left/Right_LookAround
		//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput); // doesn't need in this game
	#pragma endregion

	#pragma region LookUp/Down
		//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput); // doesn't need in this game
	#pragma endregion

	#pragma region MoveForward/Backward
			//PlayerInputComponent->BindAxis("MoveForward", this, &ACPP_Character::MoveForward);// doesn't need in this game
	#pragma endregion

	#pragma region MoveRight/Left
			PlayerInputComponent->BindAxis("MoveRight", this, &ACPP_Character::MoveRight);
	#pragma endregion

	#pragma region Jump
			PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
			PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	#pragma endregion

	#pragma region MoveFaster_MoveNormal
		PlayerInputComponent->BindAction("MoveFaster", IE_Pressed, this, &ACPP_Character::MoveFaster);
		PlayerInputComponent->BindAction("MoveFaster", IE_Released, this, &ACPP_Character::MoveSlower);
	#pragma endregion
	
	#pragma region  Fight
			PlayerInputComponent->BindAction("Punch",IE_Pressed,this,&ACPP_Character::Punch);
			PlayerInputComponent->BindAction("Kick",IE_Pressed,this,&ACPP_Character::Kick);
	#pragma endregion
}

//Move Forward and backward 
void ACPP_Character::MoveForward(float axis)
{
	const FRotator control_rotation = Controller->GetControlRotation();
	const FRotator control_rotation_yawOnly = FRotator(0, control_rotation.Yaw, 0); //FRotator( float InPitch, float InYaw, float InRoll );
	const FVector fwd = FRotationMatrix(control_rotation_yawOnly).GetUnitAxis(EAxis::X);

	AddMovementInput(fwd, axis);
}
//Move  right and left
void ACPP_Character::MoveRight(float axis)
{
	//const FRotator control_rotation = Controller->GetControlRotation();
	//const FRotator control_rotation_yawOnly = FRotator(0, control_rotation.Yaw, 0); //FRotator( float InPitch, float InYaw, float InRoll );
	//const FVector right = FRotationMatrix(control_rotation_yawOnly).GetUnitAxis(EAxis::Y);

	//AddMovementInput(right, axis);
	AddMovementInput(GetActorForwardVector(), axis);
}

void ACPP_Character::MoveFaster()
{
	GetCharacterMovement()->MaxWalkSpeed=150;
}

void ACPP_Character::MoveSlower()
{
	GetCharacterMovement()->MaxWalkSpeed=75;
}

void ACPP_Character::Punch()
{
	if(IsValid(PunchAnimMontage))
	{
		PlayAnimMontage(PunchAnimMontage,1,NAME_None);
	}
}

void ACPP_Character::Kick()
{
	if(IsValid(KickAnimMontage))
	{
		PlayAnimMontage(KickAnimMontage,1,NAME_None);
	}
}

void ACPP_Character::PunchReact()
{
	if(IsValid(PunchReactAnimMontage))
	{
		PlayAnimMontage(PunchReactAnimMontage,1,NAME_None);
	}
}


#pragma region FIGHT_INTERFACE_FUNTION
void ACPP_Character::CheckPunch_Implementation(bool isLeftHand) // to the name of method add _implementation, else it whoot not work
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Checking Punch"));
	FVector handLocation;
	if(isLeftHand)
	{
		handLocation=GetMesh()->GetBoneLocation("hand_l",EBoneSpaces::WorldSpace);
	}
	else
	{
		handLocation=GetMesh()->GetBoneLocation("hand_r",EBoneSpaces::WorldSpace);
	}
	FName hitBone = Opponent->GetClosestBone(handLocation, 20);
	
	if(hitBone != "")
	{
		Opponent->PunchReact();
	}
		
}

void ACPP_Character::CheckKick_Implementation(bool isLeftLeg) // to the name of method add _implementation, else it whoot not work
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Checking Kick"));
	
	FVector legLocation;
	if(isLeftLeg)
	{
		legLocation=GetMesh()->GetBoneLocation("ball_l",EBoneSpaces::WorldSpace);
	}
	else
	{
		legLocation=GetMesh()->GetBoneLocation("ball_r",EBoneSpaces::WorldSpace);
	}
	FName hitBone = Opponent->GetClosestBone(legLocation, 20);
	
	if(hitBone != "")
	{
		Opponent->PunchReact();
	}
	
}
#pragma endregion 

FName ACPP_Character::GetClosestBone(FVector hitBoneLocation, float maxDistance)
{
	TArray<FName> boneNames;
	 
	GetMesh()->GetBoneNames(boneNames); // add to array all bone name

	FName closestBone;
	float minDist=10000;
	
	FName bName; //currentName
	FVector boneLocation; //current bone location
	float tempDist; 
	
	for (int i = 0; i < boneNames.Num(); ++i)
	{
		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,boneNames[i].ToString());
		bName=boneNames[i];
		boneLocation = GetMesh()->GetBoneLocation(boneNames[i],EBoneSpaces::WorldSpace);
		tempDist= FVector::Dist(hitBoneLocation,boneLocation);

		if(minDist>tempDist)
		{
			minDist = tempDist;
			closestBone= boneNames[i];
		}
	}

	
	if(minDist<maxDistance)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,closestBone.ToString());
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::SanitizeFloat(minDist)); //SanitizeFloat - toString()
		return closestBone;
	}else
	{
		return "";
	}
	
}