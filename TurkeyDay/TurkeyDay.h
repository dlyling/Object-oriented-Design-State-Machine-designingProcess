/**
 * \file TurkeyDay.h
 *
 * \author Charles Owen
 *
 * Main header file for the Turkey Day application
 */

/// \cond

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTurkeyDayApp:
// See TurkeyDay.cpp for the implementation of this class
//

class CTurkeyDayApp : public CWinApp
{
public:
	CTurkeyDayApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


private:
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};

extern CTurkeyDayApp theApp;

/// \endcond