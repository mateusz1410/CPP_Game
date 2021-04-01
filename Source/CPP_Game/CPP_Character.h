// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

#include "FightInterface.h"



#include "CPP_Character.generated.h"

UCLASS()
class CPP_GAME_API ACPP_Character : public ACharacter, public IFightInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Character();	

	#pragma region CameraAndSpringArm

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			USpringArmComponent* CameraBoom;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			UCameraComponent* PlayerCamera;
	#pragma endregion
	
		UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Animations)
			UAnimMontage* PunchAnimMontage; //M_Punch
	
		UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Animations)
			UAnimMontage* KickAnimMontage; //M_Kick

		UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Animations)
			UAnimMontage* PunchReactAnimMontage; //M_PunchReact
	
		UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = SceneReferences)
			ACPP_Character* Opponent;
	

	#pragma region MoveForward_and_MoveRight
		void MoveForward(float axis);
		void MoveRight(float axis);
	#pragma endregion
	
	#pragma region MoveFaster_MoveNormal
		void MoveFaster();
		void MoveSlower();
	#pragma endregion
	
	#pragma region Fight
		void Punch();
		void Kick();
		void PunchReact();
		FName GetClosestBone(FVector hitBoneLocation, float maxDistance);// location - attacker limb (hand,leg), maxDistance - if distance is <, then there is a hit
	#pragma endregion
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//punch HIT check
	 void CheckPunch_Implementation(bool isLeftHand) override; //to the name of method add _implementation and override, else it whoot not work
	 void CheckKick_Implementation(bool isLeftLeg) override;
};