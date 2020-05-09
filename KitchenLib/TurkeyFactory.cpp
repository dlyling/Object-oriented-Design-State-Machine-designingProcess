/**
 * \file TurkeyFactory.cpp
 *
 * \author Charles Owen
 * \cond
 */

#include "stdafx.h"
#include <cassert>
#include "TurkeyFactory.h"
#include "ImageHelper.h"
#include "Turkey.h"
#include "WardrobeItem.h"

using namespace std;
using namespace Gdiplus;

std::shared_ptr<CTurkey> CTurkeyFactory::Create()
{
	return make_shared<CTurkey>(mTurkey, mTurkeyPlucked, mTurkeyBashful);
}


std::shared_ptr<CWardrobeItem> CTurkeyFactory::CreateWardrobe(CKitchenController::Wardrobe item)
{
    std::shared_ptr<CWardrobeItem> wardrobeItem;

    switch (item)
    {
    case CKitchenController::Glasses:
        wardrobeItem = make_shared<CWardrobeItem>(item, mWardrobe[item]);
        wardrobeItem->SetWardrobeOffset(Point(-40, 205));
        break;

    case CKitchenController::Cardigan:
        wardrobeItem = make_shared<CWardrobeItem>(item, mWardrobe[item]);
        wardrobeItem->SetWardrobeOffset(Point(40, 102));
        wardrobeItem->SetWardrobeScale(0.3);
        break;

    default:
        // Invalid value for clothing
        assert(false);
        break;
    }

    return wardrobeItem;
}



bool CTurkeyFactory::Load()
{
	mTurkey = CImageHelper::LoadImageW(L"turkey.png");
    mTurkeyPlucked = CImageHelper::LoadImageW(L"turkey-plucked.png");
    mTurkeyBashful = CImageHelper::LoadImageW(L"turkey-bashful.png");

	mWardrobe[CKitchenController::Glasses] = CImageHelper::LoadImageW(L"glasses.png");
	mWardrobe[CKitchenController::Cardigan] = CImageHelper::LoadImageW(L"cardigan.png");

	return mTurkey != nullptr;
}

/// \endcond

