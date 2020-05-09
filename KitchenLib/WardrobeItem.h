/**
 * \file WardrobeItem.h
 *
 * \author Charles Owen
 *
 * Class that represents one item of Turkey clothing.
 * \cond
 */

#pragma once

#include <memory>
#include "KitchenController.h"

class CWardrobeItem
{
public:
	static const double FlyingTime;

	CWardrobeItem(CKitchenController::Wardrobe item, std::shared_ptr<Gdiplus::Bitmap> image);
	~CWardrobeItem();

	void Draw(Gdiplus::Graphics * graphics, int x, int y);

	void Fly(Gdiplus::Point from, float fromScale, Gdiplus::Point to, float toScale);

	void Update(double elapsed);

	CWardrobeItem() = delete;
	CWardrobeItem(const CWardrobeItem &) = delete;
	void operator=(const CWardrobeItem &) = delete;

	CKitchenController::Wardrobe GetItem() const { return mItem; }
	void SetPosition(int x, int y) { mPosition.X = x; mPosition.Y = y; }
	bool IsFlying() { return mT < 1; }

    void SetWardrobeOffset(Gdiplus::Point offset) { mWardrobeOffset = offset; }
    Gdiplus::Point GetWardrobeOffset() const { return mWardrobeOffset; }

    void SetWardrobeScale(double s) { mWardrobeScale = (float)s; }
    float GetWardrobeScale() const { return mWardrobeScale; }

private:
    /// The item type
	CKitchenController::Wardrobe mItem;

    /// The item image
	std::shared_ptr<Gdiplus::Bitmap> mImage;

	/// Flying item
	double mT = 1;

    /// Offset to make the position align with the wardrobe image
    Gdiplus::Point mWardrobeOffset;

    /// Scale of item in the wardrobe
    float mWardrobeScale = 1.0f;

	/// Where we are flying from
	Gdiplus::Point mFrom;

    /// Scale we are flying from
    float mFromScale = 1.0f;

	/// Where we are flying to
	Gdiplus::Point mTo;

    /// Scale we are flying to
    float mToScale = 1.0f;

    /// Current position
	Gdiplus::Point mPosition;

    /// Current scale
    float mScale = 1.0f;
};

/// \endcond
