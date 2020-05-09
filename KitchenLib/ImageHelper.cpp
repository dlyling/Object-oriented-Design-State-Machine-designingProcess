/**
 * \file ImageHelper.cpp
 *
 * \author Charles Owen
 * \cond
 */

#include "stdafx.h"
#include "ImageHelper.h"
#include "Kitchen.h"
#include <string>

using namespace std;
using namespace Gdiplus;



/** 
 * Load an image into a bitmap pointer
 * \param name The filename to load 
 * \returns Reference to shared_ptr we load into
 */
std::shared_ptr<Gdiplus::Bitmap> CImageHelper::LoadImage(const std::wstring & fileName)
{
    auto path = CKitchen::ImagesDirectory + fileName;
	auto bitmap = shared_ptr<Bitmap>(Bitmap::FromFile(path.c_str()));
	if (bitmap->GetLastStatus() != Ok)
	{
		wstring msg(L"Failed to open: ");
		msg += path;
		AfxMessageBox(msg.c_str());
		return nullptr;
	}

	return bitmap;
}

/// \endcond