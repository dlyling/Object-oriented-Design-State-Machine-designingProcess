/**
 * \file Turkey.cpp
 *
 * \author Charles Owen
 * \cond
 */

#include "stdafx.h"
#include "Turkey.h"
#include "WardrobeItem.h"

using namespace Gdiplus;


CTurkey::CTurkey(std::shared_ptr<Gdiplus::Bitmap> image, 
	std::shared_ptr<Gdiplus::Bitmap> plucked,
	std::shared_ptr<Gdiplus::Bitmap> bashful) : mTurkey(image), mTurkeyPlucked(plucked), mTurkeyBashful(bashful)
{
}


CTurkey::~CTurkey()
{
}


void CTurkey::Draw(Gdiplus::Graphics *graphics, int x, int y)
{
	mPosition.X = x;
	mPosition.Y = y;

	if (mTurkey != nullptr)
	{
		auto bitmap = mTurkey;
		int wid = bitmap->GetWidth();
		int hit = bitmap->GetHeight();
		graphics->DrawImage(bitmap.get(), x - wid / 2, y - hit, wid, hit);

        // Then draw the clothing on top
        for (auto item : mClothing)
        {
            item->Draw(graphics, x, y);
        }

        if (mSay.length() > 0) 
        {
            // Finally draw any sayings
            FontFamily fontFamily(L"Arial");
            Gdiplus::Font font(&fontFamily, 30);

            StringFormat stringFormat;
            stringFormat.SetAlignment(StringAlignmentCenter);


            SolidBrush color(Color(255, 255, 0));
            graphics->DrawString(mSay.c_str(),  // String to draw
                -1,         // String length, -1 means it figures it out on its own
                &font,      // The font to use
                PointF((float)x, (float)(y-hit/2-25)),   // Where to draw (top left corner)
                &stringFormat,
                &color);    // The brush to draw the text with
        }


	}
}

/**
 * Determine if a turkey has a given wardrobe item
 * @param item Item type we are looking for
 * @return true if item is in the clothing collection
 */
bool CTurkey::HasClothing(CKitchenController::Wardrobe item)
{
    for (auto has : mClothing)
    {
        if (has->GetItem() == item)
        {
            return true;
        }
    }

    return false;
}


/**
 * Add an item to the turkey's clothing
 * \param item Item to add to the clothing collection.
 * \return true if successfully added, false if already there.
 */
bool CTurkey::Add(std::shared_ptr<CWardrobeItem> item)
{
	item->SetPosition(0, 0);
    if (!HasClothing(item->GetItem()))
    {
        mClothing.push_back(item);
        return true;
    }

    return false;
}


/**
 * Remove an item from the turkey's clothing
 * \param item Item to remove (Wardrobe enum)
 * \return wardrobe item if successful, null otherwise.
 */
std::shared_ptr<CWardrobeItem> CTurkey::Remove(CKitchenController::Wardrobe item)
{
    for (auto i=mClothing.begin(); i!=mClothing.end(); i++)
    {
        auto has = *i;
        if (has->GetItem() == item)
        {
            mClothing.erase(i);
            return has;
        }
    }

    return nullptr;
}


/**
 * Determine if the Turkey has a wardrobe malfunction
 *
 * A wardrobe malfunction is if we don't have our glasses and cardigan.
 *
 * \return true if wardrobe malfunction!
 */
bool CTurkey::WardrobeMalfunction()
{
    return mClothing.size() < 2;
}

/// \endcond