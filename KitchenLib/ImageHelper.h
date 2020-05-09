/**
 * \file ImageHelper.h
 *
 * \author Charles Owen
 *
 * Simple image loader helper class.
 * \cond
 */

#pragma once
#include <memory>
#include <string>

/**
 * Simple image loader helper class.
 */
class CImageHelper
{
public:
	CImageHelper() = delete;
	static std::shared_ptr<Gdiplus::Bitmap> LoadImage(const std::wstring &fileName);
};

/// \endcond
