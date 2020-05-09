/**
 * \file KitchenButton.cpp
 *
 * \author Charles Owen
 * \cond
 */

#include "stdafx.h"
#include "KitchenButton.h"
#include "ImageHelper.h"

const int PressedOffsetX = 3;
const int PressedOffsetY = 3;

CKitchenButton::CKitchenButton()
{
}


CKitchenButton::~CKitchenButton()
{
}


bool CKitchenButton::ButtonDown(CPoint point)
{
	if (mLoaded)
	{
		int wid = mUp->GetWidth();
		int hit = mUp->GetHeight();
		if (mEnabled && point.x >= mPosition.X - wid / 2 && point.x < mPosition.X + wid / 2 &&
			point.y >= mPosition.Y - hit / 2 && point.y < mPosition.Y + hit / 2)
		{
			mPressed = true;
			return true;
		}
	}

	return false;
}

void CKitchenButton::ButtonUp()
{
	mPressed = false;
}

/**
 * Draw the button
 * \param graphics Graphics context
 */
void CKitchenButton::Draw(Gdiplus::Graphics *graphics)
{
	if (mEnabled) 
	{
        int offsetX = mPressed ? PressedOffsetX : 0;
        int offsetY = mPressed ? PressedOffsetY : 0;
		auto bitmap = mUp;
		int wid = bitmap->GetWidth();
		int hit = bitmap->GetHeight();
		graphics->DrawImage(bitmap.get(), mPosition.X - wid / 2 + offsetX, mPosition.Y - hit / 2 + offsetY, wid, hit);
	}
}


/**
 * Load the images for the button
 * \param path Image file to load
 * \return true if successfully loaded
 */
bool CKitchenButton::Load(const std::wstring& path)
{
	mUp = CImageHelper::LoadImage(path.c_str());

	mLoaded = mUp != nullptr;
	return mLoaded;
}

/// \endcond