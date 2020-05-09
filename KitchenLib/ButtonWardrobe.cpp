/**
 * \file ButtonWardrobe.cpp
 *
 * \author Charles Owen
 * \cond
 */

#include "stdafx.h"
#include "ButtonWardrobe.h"
#include "ImageHelper.h"
#include "Kitchen.h"


CKitchenController::Wardrobe CButtonWardrobe::ButtonDown(CPoint point)
{
	if (mLoaded)
	{
		int wid = mImage->GetWidth();
		int hit = mImage->GetHeight();
		if (mEnabled && point.x >= mPosition.X - wid / 2 && point.x < mPosition.X + wid / 2 &&
			point.y >= mPosition.Y - hit / 2 && point.y < mPosition.Y + hit / 2)
		{
			// Where on the image did we click?
			int rx = point.x - (mPosition.X - wid / 2);
			int ry = point.y - (mPosition.Y - hit / 2);
			if (rx < wid / 2) 
			{
				// Left side
				return CKitchenController::Glasses;
		    }
			else
			{
				// Right side
				return CKitchenController::Cardigan;
			}

			return CKitchenController::None;
		}
	}

	return CKitchenController::None;
}



/**
* Draw the button
* \param graphics Graphics context
*/
void CButtonWardrobe::Draw(Gdiplus::Graphics *graphics)
{
	if (mEnabled)
	{
		auto bitmap = mImage;
		int wid = bitmap->GetWidth();
		int hit = bitmap->GetHeight();
		graphics->DrawImage(bitmap.get(), mPosition.X - wid / 2, mPosition.Y - hit / 2, wid, hit);
	}
}


/**
* Load the images for the button
* \param fileName Image file to load
* \return true if successfully loaded
*/
bool CButtonWardrobe::Load(const std::wstring& fileName)
{
	mImage = CImageHelper::LoadImage(fileName);
	mLoaded = mImage != nullptr;
	return mLoaded;
}

/// \endcond