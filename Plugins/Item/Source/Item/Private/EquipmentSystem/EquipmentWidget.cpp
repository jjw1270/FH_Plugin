// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentWidget.h"
#include "Item.h"
#include "Item_FHPlayerController.h"
#include "Item_PlayableCharacter.h"
#include "EquipmentComponent.h"

#include "Components/Button.h"
#include "Item_FHHUD.h"
#include "Item_HUDWidget.h"

void UEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UIDragBtn->OnPressed.AddDynamic(this, &UEquipmentWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UEquipmentWidget::OnDragBtnReleased);

	Btn_ShowHelmet->OnClicked.AddDynamic(this, &UEquipmentWidget::OnBtn_ShowHelmetClicked);
	Btn_ShowCloak->OnClicked.AddDynamic(this, &UEquipmentWidget::OnBtn_ShowCloakClicked);

	Btn_ShowHelmet->SetIsEnabled(false);
	Btn_ShowCloak->SetIsEnabled(false);

	BindEquipmentCompEvents();
}

void UEquipmentWidget::BindEquipmentCompEvents()
{
	AItem_FHPlayerController* PC = GetOwningPlayer<AItem_FHPlayerController>();

	if (IsValid(PC))
	{
		UEquipmentComponent* EquipComp = PC->GetEquipmentComp();
		if (IsValid(EquipComp))
		{
			EquipComp->ArmorUpdateDelegate.AddUObject(this, &UEquipmentWidget::OnArmorUpdate);
			return;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UEquipmentWidget::BindEquipmentCompEvents, 0.1f, false);
}

void UEquipmentWidget::OnDragBtnPressed()
{
	if (!HUDWidget)
	{
		AItem_FHHUD* HUD = Cast<AItem_FHHUD>(GetOwningPlayer()->GetHUD());
		CHECK_VALID(HUD);

		HUDWidget = HUD->GetHUDWidget();
	}

	HUDWidget->WidgetDragStart(this);
}

void UEquipmentWidget::OnDragBtnReleased()
{
	CHECK_VALID(HUDWidget);

	HUDWidget->WidgetDragEnd();
}

void UEquipmentWidget::OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateItemData, const bool& bEquip)
{
	if (UpdateArmorType == EArmorType::Helmet)
	{
		Btn_ShowHelmet->SetIsEnabled(bEquip);
		return;
	}

	if (UpdateArmorType == EArmorType::Upper)
	{
		Btn_ShowCloak->SetIsEnabled(bEquip);
		return;
	}
}

void UEquipmentWidget::OnBtn_ShowHelmetClicked()
{
	AItem_PlayableCharacter* PlayerChar = GetOwningPlayerPawn<AItem_PlayableCharacter>();
	CHECK_VALID(PlayerChar);
	
	bIsHelmetShowed = !bIsHelmetShowed;
	PlayerChar->EquipVisibilityUpdateDelegate.Broadcast(EArmorType::Helmet, bIsHelmetShowed);
}

void UEquipmentWidget::OnBtn_ShowCloakClicked()
{
	AItem_PlayableCharacter* PlayerChar = GetOwningPlayerPawn<AItem_PlayableCharacter>();
	CHECK_VALID(PlayerChar);

	bIsCloakShowed = !bIsCloakShowed;
	PlayerChar->EquipVisibilityUpdateDelegate.Broadcast(EArmorType::Upper, bIsCloakShowed);
}
