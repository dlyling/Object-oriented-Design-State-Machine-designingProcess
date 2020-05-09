#include "stdafx.h"
#include "Controller.h"

/// Location of the Wardrobe station in conveyor units
const double WardrobeStation = 1;

/// Location of the Inspection station in conveyor units
const double InspectionStation = 4;

/// Location considered to be off the screen in conveyor units
const double OffScreen = 5;

/**
 * Constructor
 */
CController::CController()
{
}

/**
 * Destructor
 */
CController::~CController()
{
}


/** Initialization function.
*
* This function is called when the system is initialized. This is
* later than when it is constructed, since the controller is not installed on
* a kitchen when it is constructed. This function is called when the kitchen
* controller is installed and ready to control a kitchen.
*/
void CController::Initialize()
{
	EnableButtonNext();
}


/** Service the kitchen.
*
* This function is called once every millisecond during kitchen
* operation. It provides a place to put code the controls the
* kitchen. Override in a local function.
*/
void CController::Service()
{
	// Increment state time by 1 millisecond
	mStateTime += 0.001;

	switch (mState)
	{
	case TurkeyNext:
	{
		if (GetTurkeyPosition() >= 1.0)
		{
			SetState(GlassDressing);
			SetState(SweaterDressing);
		}
	}
	break;

	case GlassDressing:
	{
	}
	break;

	case SweaterDressing:
	{
	}
	break;

	case Advancing:
	{
		if (GetTurkeyPosition() >= 4.0)
		{
			SetState(Advanced);
		}
	}
	break;

	case Advanced:
	{
		//SetState(Inspecting);
	}
	break;

	case Inspecting:
	{
		if (WardrobeMalfunction() != true)
		{
			SetState(Passing);
		}
		else
		{
			SetState(Returning);
		}
	}
	break;

	case Passing:
	{
		if (mStateTime >= 1.0)
		{
			Say(L"Passed");
			
			
		}
		if (mStateTime >= 2.0)
		{
			Say(L"");
			DisableButtonInspect();
			SetConveyorSpeed(1);
			SetState(Passed);
		}
		
	}
	break;

	case Passed:
	{
		if (GetTurkeyPosition() >= 5.0)
		{
			SetState(Idle);
		}
	break;
	}

	case Returning:
	{
		if (GetTurkeyPosition() <= 1.0)
		{
			SetState(GlassDressing);
		}
		if (mStateTime >= 1.0)
		{
			Say(L"");
			DisableButtonInspect();
			SetConveyorSpeed(-1);
		}
	}
	break;
	}
}



/**
* Indication that the Next Turkey button has been pressed
*
* This function is called whenever the Next On List button is pressed.
* It is only called if the button is enabled.
*/
void CController::OnNextPressed()
{
	switch (mState)
	{
	case Idle:
		SetState(TurkeyNext);
		break;
	}
}


/**
* Indication that the Advance button has been pressed
*
* This function is called whenever the Advance button is pressed.
* It is only called if the button is enabled.
*/
void CController::OnAdvancePressed()
{
	switch (mState)
	{
	case GlassDressing:
		SetState(Advancing);
		break;

	case SweaterDressing:
		SetState(Advancing);
		break;
	}
}


/**
* Indication that a Wardrobe button has been pressed
* \param item The item that was selected
*/
void CController::OnWardrobePressed(CKitchenController::Wardrobe item)
{
	switch (mState)
	{
	case GlassDressing:
		if (HasClothing(item))
		{
			UndressTurkey(item);
		}
		else
		{
			DressTurkey(item);
		}
		break;

	case SweaterDressing:
		if (HasClothing(item))
		{
			UndressTurkey(item);
		}
		else
		{
			DressTurkey(item);
		}
		break;
	}
}

/**
* Called when the Turkey has been dressed in a clothing item
*
* The process is started by a call to DressTurkey() or UndressTurkey()
* This function is called when the process is complete.
*/
void CController::WardrobeDone()
{
	
}



/**
* Indication that the Inspect button has been pressed
*
* This function is called whenever the Inspect button is pressed.
* It is only called if the button is enabled.
*/
void CController::OnInspectPressed()
{
	switch (mState)
	{
	case Advanced:
	{
		SetState(Inspecting);
	}
	break;
	}
}

/** Set a state
 * \param state The new state to set
 */
void CController::SetState(CController::States state)
{
	mState = state;
	mStateTime = 0;

	// Entry activities for states
	switch (mState)
	{
	case Idle:
		EnableButtonNext();
		break;

	case TurkeyNext:
		NextTurkey();
		SetConveyorSpeed(1);
		DisableButtonNext();
		EnableButtonAdvance();
		EnableButtonWardrobe();
		break;

	case GlassDressing:
		SetConveyorSpeed(0);
		EnableButtonAdvance();
		EnableButtonWardrobe();
		break;

	case SweaterDressing:
		SetConveyorSpeed(0);
		EnableButtonAdvance();
		EnableButtonWardrobe();
		break;

	case Advancing:
		SetConveyorSpeed(1);
		DisableButtonAdvance();
		DisableButtonWardrobe();
		break;

	case Advanced:
		SetConveyorSpeed(0);
		EnableButtonInspect();
		break;

	case Inspecting:
		Say(L"Inspecting");
		break;

	case Returning:

		break;

	case Passing:
		DisableButtonInspect();
		break;

	case Passed:
		break;
	}
}
