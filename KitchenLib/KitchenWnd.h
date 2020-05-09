/**
 * \file KitchenWnd.h
 * Kitchen/Turkey simulation class
 *
 * \author Charles Owen
 * \version 1.01 11-25-2015 Initial Development
 * \version 1.02 11-21-2017 Minion Morning version
 * \version 1.03 11-17-2019 Turkey Day version
 */

#pragma once
#include <memory>
#include <string>
#include <vector>

#include "KitchenButton.h"
#include "ButtonWardrobe.h"
#include "TurkeyFactory.h"
#include "Turkey.h"

#include "KitchenController.h"
#include "TimeManagement.h"

/**
 * Kitchen simulation class
 *
 * This class simulates the morning ritual for a Turkey.
 * We have to cloth our Turkey so he can face the day.
 *
 * You do not directly access the functions in CKitchenWnd.
 * Instead, use the functions in CKitchenController for 
 * managing all kitchen operations.
 */
class AFX_EXT_CLASS CKitchenWnd : public CWnd
{
public:
    /**
    * Constructor
    */
	CKitchenWnd();

    /**
    * Destructor
    */
	virtual ~CKitchenWnd();
	
	/**
	 * Set a controller for this window
	 * \param c Installed controller */
	void SetController(std::shared_ptr<CKitchenController> c);

	/// Set the display scaling
	/// \param scale Display scaling
	void SetScale(double scale) { mScale = scale; }

	/// Get the current display scaling
	/// \return Scale
	double GetScale() const { return mScale; }

    /**
    * Update the simulation based on time
    *
    * This is normally only called from testing. It advances the simulation
    * by some elapsed time. Note that this does not have to be
    * real time.
    *
    * This will advance in millisecond increments only.
    *
    * \param elapsed Time to advance the simulation in seconds.
    */
    void Update(double elapsed);

    /**
     * Load all images from files
     *
     * This is only used for Test, since it is automatically
     * called when the window is created when not in test
     */
    void Load();

    /// \cond

	//
	// Getters
	//

    int GetWidth();
    int GetHeight();

	/** Get the turkey position
	*
	* <1 means to the left of the short, >4 is to the right of inspection
	* The positions of the stages are 1, 2, 3, and 4
	* \return Position relative to the stages */
	double GetTurkeyPosition() { return mTurkeyPosition; }

	/// Get the current Turkey
	/// \return Pointer to current Turkey
	std::shared_ptr<CTurkey> GetTurkey() { return mTurkey; }

	//
	// Activities
	//

	/** Enable the Next Turkey button
	 * \param enable true to enable */
	void EnableButtonNext(bool enable) { mButtonNext.SetEnabled(enable); }


	/** Enable the Advance button
	* \param enable true to enable */
	void EnableButtonAdvance(bool enable) { mButtonAdvance.SetEnabled(enable); }

	/** Enable the Wardrobe button
	 * \param enable true to enable */
	void EnableButtonWardrobe(bool enable) { mButtonWardrobe.SetEnabled(enable); }

    /** Enable the Inspect button
	* \param enable true to enable */
	void EnableButtonInspect(bool enable) { mButtonInspect.SetEnabled(enable); }

	/** Apply a wardrobe item to the turkey
	* \param item Wardrobe item to apply */
	void DressTurkey(CKitchenController::Wardrobe item);

    /** Remove a wardrobe item from the turkey
    * \param item Wardrobe item to apply */
    void UndressTurkey(CKitchenController::Wardrobe item);

    /**
    * Set the speed of the conveyor belt
    *
    * \param speed Speed to set. Positive values move the bag
    * to the right, negative values to the left. Values should
    * range from -1 to 1 (max left speed to max right speed)
    */
    void SetConveyorSpeed(double speed);

	/**
	 * Add the next Turkey to the conveyor belt end
	 * Position is set to -1, speed is set to 0
	 */
	void NextTurkey();

	//
	// Testing support
	//
	CKitchenButton *GetButtonNext() { return &mButtonNext; }
	CKitchenButton *GetButtonAdvance() { return &mButtonAdvance; }
	CButtonWardrobe *GetButtonWardrobe() { return &mButtonWardrobe; }
	CKitchenButton *GetButtonInspect() { return &mButtonInspect; }

	bool IsUndressing() { return mUndressingItem != nullptr; }

private:
    DECLARE_DYNAMIC(CKitchenWnd)

protected:

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);


private:
    void Draw(Gdiplus::Graphics *graphics, std::shared_ptr<Gdiplus::Bitmap> &bitmap, int x, int y);
    void DrawButtons(Gdiplus::Graphics *graphics);
	void ButtonsUp();

    void TimeManagement();
    void UpdateTimeslice();

	double mScale = 1.0;				///< Display scale

	bool mLButtonDown = false;			///< Left button is down?

	//
	// Buttons
	//
	CKitchenButton mButtonNext;				///< Next Turkey button
	CKitchenButton mButtonAdvance;			///< Advance button
	CButtonWardrobe mButtonWardrobe;	    ///< Wardrobe button
	CKitchenButton mButtonInspect;			///< Inspect button

    bool mFirstDraw = true;				///< True for the first time we draw

	CTurkeyFactory mTurkeyFactory;		///< The Turkey generating factory

    // A controller for this kitchen
    std::shared_ptr<CKitchenController> mController;

    /// Manage the simulation timing
    CTimeManagement mTimeManagement;

    double   mTime = 0;             ///< Local time

	/// Background image
    std::shared_ptr<Gdiplus::Bitmap>  mBackgroundImage;

	//
	// Conveyor belt and moving Turkey
	//
	double mConveyorSpeed = 0;		///< Belt speed in stations per second
	double mTurkeyPosition = -1;		///< Current Turkey position in stations
	std::shared_ptr<CTurkey> mTurkey;	///< Active Turkey

	std::shared_ptr<CWardrobeItem> mDressingItem;	///< Flying on wardrobe item
	std::shared_ptr<CWardrobeItem> mUndressingItem;	///< Flying off wardrobe item

    /// \endcond
};


