/**
 * \file KitchenController.cpp
 *
 * \author Charles Owen
 */

#include "stdafx.h"
#include "KitchenController.h"
#include "KitchenWnd.h"
#include "Turkey.h"

#include <cassert>




CKitchenController::CKitchenController()
{
}


CKitchenController::~CKitchenController()
{
}


double CKitchenController::GetTurkeyPosition()
{
	assert(mKitchen != NULL);
	return mKitchen->GetTurkeyPosition();
}


void CKitchenController::NextTurkey()
{
	assert(mKitchen != NULL);
	mKitchen->NextTurkey();
}


void CKitchenController::SetConveyorSpeed(double speed)
{
    assert(mKitchen != NULL);
    mKitchen->SetConveyorSpeed(speed);
}


void CKitchenController::EnableButtonNext()
{
	assert(mKitchen != NULL);
	mKitchen->EnableButtonNext(true);
}


void CKitchenController::DisableButtonNext()
{
	assert(mKitchen != NULL);
	mKitchen->EnableButtonNext(false);
}

void CKitchenController::EnableButtonAdvance()
{
	assert(mKitchen != NULL);
	mKitchen->EnableButtonAdvance(true);
}

void CKitchenController::DisableButtonAdvance()
{
	assert(mKitchen != NULL);
	mKitchen->EnableButtonAdvance(false);
}

void CKitchenController::EnableButtonWardrobe()
{
	assert(mKitchen != NULL);
	mKitchen->EnableButtonWardrobe(true);
}


void CKitchenController::DisableButtonWardrobe()
{
	assert(mKitchen != NULL);
	mKitchen->EnableButtonWardrobe(false);
}

void CKitchenController::EnableButtonInspect()
{
	assert(mKitchen != NULL);
	mKitchen->EnableButtonInspect(true);
}


void CKitchenController::DisableButtonInspect()
{
	assert(mKitchen != NULL);
	mKitchen->EnableButtonInspect(false);
}

void CKitchenController::DressTurkey(CKitchenController::Wardrobe item)
{
	assert(mKitchen != NULL);
	mKitchen->DressTurkey(item);
}

void CKitchenController::UndressTurkey(CKitchenController::Wardrobe item)
{
    assert(mKitchen != NULL);
    mKitchen->UndressTurkey(item);
}

bool CKitchenController::WardrobeMalfunction()
{
	assert(mKitchen != nullptr);
	auto turkey = mKitchen->GetTurkey();
	assert(turkey != nullptr);
	return turkey->WardrobeMalfunction();
}

bool CKitchenController::AnyClothes()
{
	assert(mKitchen != nullptr);
	auto turkey = mKitchen->GetTurkey();
	assert(turkey != nullptr);
	return turkey->AnyClothes();
}

bool CKitchenController::HasClothing(CKitchenController::Wardrobe item)
{
    assert(mKitchen != nullptr);
    auto turkey = mKitchen->GetTurkey();
    assert(turkey != nullptr);
    return turkey->HasClothing(item);
}


void CKitchenController::Say(const std::wstring& say)
{
    assert(mKitchen != nullptr);
    auto turkey = mKitchen->GetTurkey();
    assert(turkey != nullptr);
    turkey->Say(say);
}

void CKitchenController::Press(Buttons buttons)
{
    if (buttons & Advance)
    {
        OnAdvancePressed();
    }

    if (buttons & Next)
    {
        OnNextPressed();
    }

    if (buttons & Inspect)
    {
        OnInspectPressed();
    }

    if (buttons & Wardrobes)
    {
        OnWardrobePressed(Glasses);
        OnWardrobePressed(Cardigan);
    }
}

void CKitchenController::PressExcept(Buttons buttons)
{
    Press((Buttons)(~buttons));
}


/// \cond
void CKitchenController::SetKitchen(CKitchenWnd* workshop)
{
    mKitchen = workshop;
}
/// \endcond