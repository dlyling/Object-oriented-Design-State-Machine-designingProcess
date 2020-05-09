/**
* \file KitchenController.h
* Kitchen controller abstract base class.
*
* This class is the base class for a kitchen controller class.
* That class provides the kitchen control.
*
*  \author Charles B. Owen
*/

#pragma once

#include <string>

class CKitchenWnd;

/**
* Kitchen controller base class.
*
* Kitchen controllers are derived from this base class.
*/
class AFX_EXT_CLASS CKitchenController
{
public:
    /** Constructor */
    CKitchenController();

    /** Destructor */
    virtual ~CKitchenController();

    /** Copy constructor disabled */
    CKitchenController(const CKitchenController &) = delete;
    /** Assignment operator disabled */
    void operator=(const CKitchenController &) = delete;

    /// Enum that defines the possible wardrobe options
    /// Only Glasses and Cardigan are supported.
    enum Wardrobe { None = -1, Glasses = 0, Cardigan = 1, NumWardrobe = 2 };

    /// Enum that assigns values to buttons
    enum Buttons {Next = 1, Advance = 2, Inspect = 4, Wardrobes = 8};

	/// \cond
	void SetKitchen(CKitchenWnd *kitchen);
	/// \endcond

    /** 
    * Initialization function.
    *
    * This function is called when the system is initialized. This is
    * later than when it is constructed, since the controller is not installed on
    * a kitchen when it is constructed. This function is called when the kitchen
    * controller is installed and ready to control a kitchen.
    */
    virtual void Initialize() {}


	/** Service the kitchen.
	*
	* This function is called once every millisecond during kitchen
	* operation. It provides a place to put code the controls the
	* kitchen. Override in a local function.
	*/
	virtual void Service() {};

	/** 
    * Get the Kitchen this controller is for
	* \returns Pointer to CKitchenWnd object 
    */
	CKitchenWnd *GetKitchen() { return mKitchen; }

	//
	// Getters
	//

	/** 
    * Get the turkey position
	*
	* less than 1 means to the left of the short, 
    * greater than 4 is to the right of inspection
	* The positions of the stages are 1, 2, 3, and 4
	* \return Position relative to the stages 
    */
	double GetTurkeyPosition();

	/** Does the Turkey have a wardrobe malfunction?
	 * \returns true if Wardrobe malfunction!
     */
	bool WardrobeMalfunction();

	/// Is the Turkey wearing any clothes?
	/// \returns true if any clothes are there
	bool AnyClothes();

    /// Does the Turkey have this clothing item?
    /// \param item Item to check (enum)
    /// \return True if Turkey is wearing that item
    bool HasClothing(CKitchenController::Wardrobe item);

	//
	// Events
	//

	/**
	* Indication that the Next Turkey button has been pressed
	*
	* This function is called whenever the Next Turkey button is pressed.
	* It is only called if the button is enabled.
	*/
	virtual void OnNextPressed() {}

	/**
	* Indication that the Advance button has been pressed
	*
	* This function is called whenever the Advance button is pressed.
	* It is only called if the button is enabled.
	*/
	virtual void OnAdvancePressed() {}

	/**
	* Indication that the Inspect button has been pressed
	*
	* This function is called whenever the Inspect button is pressed.
	* It is only called if the button is enabled.
	*/
	virtual void OnInspectPressed() {}

	/**
	* Indication that a Wardrobe button has been pressed
	* \param item The item that was selected
	*/
	virtual void OnWardrobePressed(Wardrobe item) {}

	/**
	* Called when a wardrobe operation has been completed for our Turkey.
	*
	* The process is started by a call to DressTurkey() or UndressTurkey()
	* This function is called when the process is complete.
	*/
	virtual void WardrobeDone() {}


	//
	// Activities
	//


	/**
	 * Add a new turkey to the conveyor belt
	 */
	void NextTurkey();

	/**
	* Set the speed of the bag conveyer belt
	*
	* \param speed Speed to set. Positive values move the bag
	* to the right, negative values to the left. Values should
	* range from -1 to 1 (max left speed to max right speed)
	*/
	void SetConveyorSpeed(double speed);

	/// Enable the Next Turkey button
	void EnableButtonNext();

	/// Disable the Next Turkey button
	void DisableButtonNext();

	/// Enable the Advance button
	void EnableButtonAdvance();

	/// Disable the Advance button
	void DisableButtonAdvance();

	/// Enable the Wardrobe button
	void EnableButtonWardrobe();

	/// Disable the Wardrobe button
	void DisableButtonWardrobe();

	/// Enable the Inspect button
	void EnableButtonInspect();

	/// Disable the Inspect button
	void DisableButtonInspect();

	/** Apply a wardrobe item to the turkey
	 * \param item Wardrobe item to apply */
	void DressTurkey(Wardrobe item);

    /** Remove a wardrobe item from the turkey
     * \param item Wardrobe item to remove */
    void UndressTurkey(Wardrobe item);

    /** Say something over the turkey
     * \param say What to say
     */
    void Say(const std::wstring& say);

    /** 
     * Press multiple buttons
     * Used for testing purposes only 
     * \param buttons Or combination of Buttons enum values.
     */
    void Press(Buttons buttons);

    /**
     * Press multiple buttons other than specified ones
     * Used for testing purposes only
     * \param buttons Or combination of Buttons enum values.
     */
    void PressExcept(Buttons buttons);

private:
    /// The kitchen we are controlling
    CKitchenWnd *mKitchen = nullptr;
};

