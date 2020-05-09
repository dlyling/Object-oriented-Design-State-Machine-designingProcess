/**
 * \file KitchenWnd.cpp
 *
 * \author Charles Owen
 */

#include "stdafx.h"
#include <algorithm>    // std::random_shuffle

#include "WorkshopLib.h"
#include "KitchenWnd.h"
#include "DoubleBufferDC.h"
#include "Turkey.h"
#include "WardrobeItem.h"
#include "Kitchen.h"
#include "ImageHelper.h"


#include <cassert>

using namespace std;
using namespace Gdiplus;

/// \cond 

// Frame duration in milliseconds
const int FrameDuration = 30;

/* Pixel values */
const int KitchenWidth = 800;
const int KitchenHeight = 600;

const int NumStations = 4;

// Y value for the bottom row buttons
const int ButtonY = 550;

// Y value for the Wardrobe button (larger)
const int ButtonWardrobeY = ButtonY - 49;

// Y value for the bottom of a Turkey
const int TurkeyY = 405;

// Duration of a time slice for our simulation
const double SimulationTimeslice = 0.001;

// Maximum conveyor belt speed in stations per second
const double MaxConveyorSpeed = 1;

IMPLEMENT_DYNAMIC(CKitchenWnd, CWnd)

/// \endcond

/** 
 * Constructor 
 */
CKitchenWnd::CKitchenWnd()
{
    srand((unsigned int)time(nullptr));


}

/// Destructor
CKitchenWnd::~CKitchenWnd()
{
}


void CKitchenWnd::Update(double elapsed)
{
    while (elapsed >= SimulationTimeslice)
    {
        UpdateTimeslice();
        elapsed -= SimulationTimeslice;
    }
}


void CKitchenWnd::Load()
{
    //
    // Load all of the images
    //
    mBackgroundImage = CImageHelper::LoadImage(L"kitchen.jpg");
    mButtonAdvance.Load(L"advance.png");
	mButtonAdvance.SetPosition(650, 48);
	mButtonNext.Load(L"next-turkey.png");
	mButtonNext.SetPosition(650, 48);
	mButtonInspect.Load(L"inspect.png");
	mButtonInspect.SetPosition(700, ButtonY);
	mButtonWardrobe.Load(L"wardrobe.png");
	mButtonWardrobe.SetPosition(100, ButtonWardrobeY);

	mTurkeyFactory.Load();
}

/// \cond


BEGIN_MESSAGE_MAP(CKitchenWnd, CWnd)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


/** The workshop width. Based on the width of the background image
* \return Width in pixels */
int CKitchenWnd::GetWidth()
{
    return (int)(KitchenWidth * mScale);
}

/** The workshop height. Based on the height of the background image
* \return Height in pixels */
int CKitchenWnd::GetHeight()
{
    return (int)(KitchenHeight * mScale);
}

void CKitchenWnd::OnPaint()
{
    if (mFirstDraw)
    {
        SetTimer(1, FrameDuration, nullptr);
        mFirstDraw = false;
    }

    // Handle time management for the simulation
    TimeManagement(); 

    CPaintDC paintDC(this); // device context for painting

    CDoubleBufferDC dbDC(&paintDC);

    Graphics graphics(dbDC.m_hDC);    // Create GDI+ graphics context
	graphics.ScaleTransform((REAL)mScale, (REAL)mScale);

    int w, h;
    CRect rect;
    GetClientRect(&rect);
    w = rect.Width();
    h = rect.Height();
    int bottom = mBackgroundImage->GetHeight();

    // 
    // Background
    //

    Draw(&graphics, mBackgroundImage, 0, 0);

    //
    // Buttons
    //
    DrawButtons(&graphics);

	if (mTurkey != nullptr)
	{
		mTurkey->Draw(&graphics, (int)((mTurkeyPosition - 0.5) * KitchenWidth / NumStations), TurkeyY);
	}

	if (mDressingItem != nullptr)
	{
		mDressingItem->Draw(&graphics, 0, 0);
	}

	if (mUndressingItem != nullptr)
	{
		mUndressingItem->Draw(&graphics, 0, 0);
	}

}


void CKitchenWnd::DrawButtons(Gdiplus::Graphics *graphics)
{
	mButtonNext.Draw(graphics);
	mButtonAdvance.Draw(graphics); 
	mButtonInspect.Draw(graphics);
	mButtonWardrobe.Draw(graphics);
}





void CKitchenWnd::OnTimer(UINT_PTR nIDEvent)
{
    Invalidate();
    UpdateWindow();

    CWnd::OnTimer(nIDEvent);
}


void CKitchenWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	point.x = (int)(point.x / mScale);
	point.y = (int)(point.y / mScale);

	mLButtonDown = true;

    //
    // Standard buttons
    //
	if (mButtonNext.ButtonDown(point))
	{
		mController->OnNextPressed();
		return;
	}

	if (mButtonAdvance.ButtonDown(point))
	{
		mController->OnAdvancePressed();
		return;
	}

	if (mButtonInspect.ButtonDown(point))
	{
		mController->OnInspectPressed();
		return;
	}

	auto ret = mButtonWardrobe.ButtonDown(point);
	if (ret != CKitchenController::None)
	{
		mController->OnWardrobePressed(ret);
		return;
	}

    CWnd::OnLButtonDown(nFlags, point);
}

void CKitchenWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	point.x = (int)(point.x / mScale);
	point.y = (int)(point.y / mScale);


	if (mLButtonDown)
	{
		ButtonsUp();
	}

	CWnd::OnLButtonUp(nFlags, point);
}


void CKitchenWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	point.x = (int)(point.x / mScale);
	point.y = (int)(point.y / mScale);

	if (!(nFlags & MK_LBUTTON) && mLButtonDown)
	{
		ButtonsUp();
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CKitchenWnd::ButtonsUp()
{
	mLButtonDown = false;
	mButtonAdvance.ButtonUp();
	mButtonNext.ButtonUp();
}


int CKitchenWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    Load();



    return 0;
}




BOOL CKitchenWnd::OnEraseBkgnd(CDC* pDC)
{
    return FALSE;
}


/**
* Draw a bitmap it's native size.
* \param bitmap Bitmap declared as a shared_ptr to a Gdiplus::Bitmap
* \param x X location in pixels
* \param y Y location in pixels
*/
void CKitchenWnd::Draw(Gdiplus::Graphics *graphics, std::shared_ptr<Gdiplus::Bitmap> &bitmap, int x, int y)
{
    graphics->DrawImage(bitmap.get(), x, y, bitmap->GetWidth(), bitmap->GetHeight());
}

/**
* Set the elevator controller we are using for this elevator.
* @param c Elevator controller
*/
void CKitchenWnd::SetController(std::shared_ptr<CKitchenController> c)
{
    mController = c;
    mController->SetKitchen(this);
    mController->Initialize();
}


/** Handle once-per-draw time management
*
* Handles updating the time management component and
* calls to the local Update function.
*/
void CKitchenWnd::TimeManagement()
{
    mTimeManagement.Update();

    while (mTime < mTimeManagement.GetTime())
    {
        UpdateTimeslice();
        mTime += SimulationTimeslice;
    }

}


/** Update in time
*/
void CKitchenWnd::UpdateTimeslice()
{
    double elapsed = SimulationTimeslice;
	
	mTurkeyPosition += mConveyorSpeed * elapsed;
	if (mTurkeyPosition >= 5.0)
	{
		// Turkey has left the building
		mTurkey = nullptr;
	}
	
	if (mDressingItem != nullptr && mDressingItem->IsFlying())
	{
		mDressingItem->Update(elapsed);
		if (!mDressingItem->IsFlying()) {
			mTurkey->Add(mDressingItem);
			mDressingItem = nullptr;
			mController->WardrobeDone();
		}
	}

	if (mUndressingItem != nullptr && mUndressingItem->IsFlying())
	{
		mUndressingItem->Update(elapsed);
		if (!mUndressingItem->IsFlying()) {
			mUndressingItem = nullptr;
            mController->WardrobeDone();
		}
	}

    if (mController != nullptr)
    {
        mController->Service();
    }

}

void CKitchenWnd::NextTurkey()
{
	mTurkey = mTurkeyFactory.Create();
    // Example of how to add clothing items to the Turkey:
    // auto glasses = mTurkeyFactory.CreateWardrobe(CKitchenController::Glasses);
    // mTurkey->Add(glasses);
    // auto cardigan = mTurkeyFactory.CreateWardrobe(CKitchenController::Cardigan);
    // mTurkey->Add(cardigan);

	mTurkeyPosition = 0;
	SetConveyorSpeed(0);
}

void CKitchenWnd::SetConveyorSpeed(double speed)
{
    // Actual stored speed is in stations per second
	mConveyorSpeed = speed * MaxConveyorSpeed;
}

void CKitchenWnd::DressTurkey(CKitchenController::Wardrobe item)
{
	assert(mTurkey != nullptr);
	mDressingItem = mTurkeyFactory.CreateWardrobe(item);
	mDressingItem->Fly(mButtonWardrobe.GetPosition() + mDressingItem->GetWardrobeOffset(), mDressingItem->GetWardrobeScale(), 
        mTurkey->GetPosition(), 1.0f);
}

void CKitchenWnd::UndressTurkey(CKitchenController::Wardrobe item)
{
    assert(mTurkey != nullptr);
    mUndressingItem = mTurkey->Remove(item);
    assert(mUndressingItem != nullptr);
    mUndressingItem->Fly(mTurkey->GetPosition(), 1.0f, 
        mButtonWardrobe.GetPosition() + mUndressingItem->GetWardrobeOffset(), mUndressingItem->GetWardrobeScale());
}

/// \endcond


