/**
 * \file Turkey.h
 *
 * \author Charles Owen
 *
 * Class for a turkey we are dressing...
 * \cond
 */

#pragma once

#include <memory>
#include <vector>
#include "KitchenController.h"

class CWardrobeItem;

/**
 * Class for a turkey we are dressing...
 */
class AFX_EXT_CLASS CTurkey
{
public:
	CTurkey(std::shared_ptr<Gdiplus::Bitmap> image, 
		std::shared_ptr<Gdiplus::Bitmap> plucked,
		std::shared_ptr<Gdiplus::Bitmap> bashful);
	CTurkey() = delete;
	CTurkey(const CTurkey &) = delete;
	~CTurkey();

	void Draw(Gdiplus::Graphics * graphics, int x, int y);

    bool HasClothing(CKitchenController::Wardrobe item);

	bool Add(std::shared_ptr<CWardrobeItem> item);

    std::shared_ptr<CWardrobeItem> Remove(CKitchenController::Wardrobe item);

	bool WardrobeMalfunction();

    bool AnyClothes() { return !mClothing.empty(); }

    void Say(const std::wstring& say) { mSay = say; }

    const std::wstring& Saying() const { return mSay; }

    /// Get the current Turkey position
    /// \return Position of the Turkey
    Gdiplus::Point GetPosition() { return mPosition; }

private:
	Gdiplus::Point mPosition;

    /// All of the Turkey wardrobe items
    /// Right now that is just glasses and cardigan
    std::vector<std::shared_ptr<CWardrobeItem>> mClothing;

    /// Something we are saying about our Turkey
    std::wstring mSay;

	std::shared_ptr<Gdiplus::Bitmap> mTurkey;
	std::shared_ptr<Gdiplus::Bitmap> mTurkeyPlucked;
	std::shared_ptr<Gdiplus::Bitmap> mTurkeyBashful;
};

/// \endcond