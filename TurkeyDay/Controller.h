/**
 * \file Controller.h
 *
 * \author Charles Owen
 *
 * Main controller for the Turkey Day system
 */

#include "KitchenController.h"

#pragma once

/**
 * Main controller for the Turkey Day system
 */
class CController :
    public CKitchenController
{
public:
    CController();
    virtual ~CController();
    
    virtual void Initialize() override;
    virtual void Service() override;

	virtual void OnNextPressed() override;
	virtual void OnAdvancePressed() override;
	virtual void OnWardrobePressed(CKitchenController::Wardrobe item) override;
	virtual void WardrobeDone() override;
	virtual void OnInspectPressed() override;

	/// The state machine states
	enum States { Idle, TurkeyNext, GlassDressing, SweaterDressing, Advancing, Advanced, Inspecting, Passing,Passed, Returning };

private:
	States mState = Idle;   ///< The current state

	void SetState(CController::States state);  ///< Set a state

	double mStateTime = 0;  ///< The time in a current state

};

