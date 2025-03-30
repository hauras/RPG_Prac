
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RpgDebugHelper.h"

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f); // 너비, 높이

	// 컨트롤러 회전에 의해 회전하지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
    //안녕
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 200.f;
    CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("Working"));
}
