/**
 * \file ButtonWardrobe.h
 *
 * \author Charles Owen
 *
 * The Wardrobe button. It's special since it has four 
 * possible return values on a hit.
 * \cond
 */

#pragma once
#include "KitchenController.h"

#include <memory>


/**
 * The Wardrobe button. It's special since it has four
 * possible return values on a hit.
 */
class AFX_EXT_CLASS CButtonWardrobe
{
public:
	CKitchenController::Wardrobe ButtonDown(CPoint point);

	void Draw(Gdiplus::Graphics *graphics);

	bool Load(const std::wstring& fileName);

	int GetHeight() const { return mImage->GetHeight(); }



	/**
	* Set the position of the center of the button
	* \param x X pixel location
	* \param y Y pixel location
	*/
	void SetPosition(int x, int y) { mPosition = Gdiplus::Point(x, y); }

	/// Get the postitino of the center of the button
	/// \return position on the screen
	Gdiplus::Point GetPosition() { return mPosition; }

	/** Set the enabled status for the button
	* \param enabled Enabled status */
	void SetEnabled(bool enabled) { mEnabled = enabled; }

	bool IsEnabled() const { return mEnabled; }

private:
	bool mLoaded = false;
	bool mEnabled = false;

	Gdiplus::Point mPosition;

	std::shared_ptr<Gdiplus::Bitmap>  mImage;
};

/// \endcond