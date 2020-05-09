/**
 * \file TurkeyFactory.h
 *
 * \author Charles Owen
 *
 * Factory class that makes Turkey objects
 * \cond
 */

#pragma once

#include <memory>
#include "KitchenController.h"

class CTurkey;
class CWardrobeItem;

/**
 * Factory class that makes Turkey objects
 *
 * This is an unusual factory, since we are going to keep it 
 * around all of the time. We do this because it holds pointers
 * to all of the images that are used so we only have to 
 * load them one time.
 */
class CTurkeyFactory
{
public:
	std::shared_ptr<CTurkey> Create();
	std::shared_ptr<CWardrobeItem> CreateWardrobe(CKitchenController::Wardrobe item);

	bool Load();

private:
	std::shared_ptr<Gdiplus::Bitmap> mTurkey;
    std::shared_ptr<Gdiplus::Bitmap> mTurkeyPlucked;
    std::shared_ptr<Gdiplus::Bitmap> mTurkeyBashful;

	/// Wardrobe item images
	std::shared_ptr<Gdiplus::Bitmap>  mWardrobe[CKitchenController::NumWardrobe];
};

/// \endcond