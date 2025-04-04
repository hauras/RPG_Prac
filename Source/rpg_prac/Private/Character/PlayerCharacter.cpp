
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/RpgInputComponent.h"
#include "RpgGameplayTags.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
#include "Components/Combat/PlayerCombatComponent.h"


#include "RpgDebugHelper.h"

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f); // 너비, 높이

	// 컨트롤러 회전에 의해 회전하지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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

    PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (!CharacterStartUpData.IsNull())
    {
        if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
        {
            LoadedData->GiveToAbilitySystemComponent(CharacterAbilitySystemComponent);
        }
    }
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));
    ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
    
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
    check(Subsystem);

    Subsystem->AddMappingContext(InputConfigDataAsset->InputContext, 0);

    URpgInputComponent* RpgInputComponent = CastChecked<URpgInputComponent>(PlayerInputComponent);

    RpgInputComponent->BindNativeInputAction(InputConfigDataAsset, RpgGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
    RpgInputComponent->BindNativeInputAction(InputConfigDataAsset, RpgGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
    RpgInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

    const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

    if (MovementVector.Y != 0.f)
    {
        const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

        AddMovementInput(ForwardDirection, MovementVector.Y);
    }

    if (MovementVector.X != 0.f)
    {
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
    const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

    if (LookAxisVector.X != 0.f)
    {
        AddControllerYawInput(LookAxisVector.X);
    }

    if (LookAxisVector.Y != 0.f)
    {
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
    CharacterAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
    CharacterAbilitySystemComponent->OnAbilityInputReleased(InInputTag);

}
