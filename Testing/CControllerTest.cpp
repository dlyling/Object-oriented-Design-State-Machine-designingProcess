#include "stdafx.h"
#include "CppUnitTest.h"
#include "KitchenWnd.h"
#include "Controller.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Testing
{
	TEST_CLASS(CControllerTest)
	{
	public:
        /**
         * Initialization so image loading will work
         */
        TEST_METHOD_INITIALIZE(methodName)
        {
            extern wchar_t g_dir[];
            g_dir[999] = 0;
            ::SetCurrentDirectory(g_dir);
        }

        /**
         * Test that we can simply set a controller
         */
        TEST_METHOD(TestCControllerSetController)
        {
            // Create a test object
            CKitchenWnd kitchen;
            kitchen.Load();

            // Create a mock controller and install it
            auto controller = std::make_shared<CController>();

            kitchen.SetController(controller);
            Assert::IsTrue(controller->GetKitchen() == &kitchen);
        }

        /**
         * This tests that you can press Advance and get a Turkey
         * loaded onto the conveyer belt and to the wardrobe station. 
		 * This should be the first test you try to pass.
         */
        TEST_METHOD(TestCControllerTurkeyToWardrobe)
        {
            // Create a test object
            CKitchenWnd kitchen;
            kitchen.Load();

            // Create a controller and install it
            auto controller = std::make_shared<CController>();
            kitchen.SetController(controller);

            // Initially there is no Turkey
            Assert::IsTrue(kitchen.GetTurkey() == nullptr);

            // Ensure Next Turkey button is enabled
            Assert::IsTrue(kitchen.GetButtonNext()->IsEnabled());

            // Ensure Advance button is not enabled
            Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());

            // Press the other buttons to be sure they are ignored
            // This should not break anything
            controller->PressExcept(CController::Next);

            // Ensure Next Turkey button is enabled
            Assert::IsTrue(kitchen.GetButtonNext()->IsEnabled());

            // Ensure Advance button is not enabled
            Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());

            // Press the Next Turkey button
            controller->OnNextPressed();

            // It takes one second for the Turkey to move in and stop.
            // Run the workshop for a bit more than that.
            kitchen.Update(1.1);

            // Is the Turkey created and in place at the wardrobe station (1)?
            Assert::IsTrue(kitchen.GetTurkey() != nullptr);
            Assert::AreEqual(1, controller->GetTurkeyPosition(), 0.01);

            // Press the other buttons to be sure they are ignored
            // This should not break anything
            controller->PressExcept(CController::Advance);

            // Is the Turkey created and in place at the wardrobe station (1)?
            Assert::IsTrue(kitchen.GetTurkey() != nullptr);
            Assert::AreEqual(1, controller->GetTurkeyPosition(), 0.01);
        }



        /**
         * This tests this process:
         * Turkey moves to wardrobe
         * Both wardrobe items are added to the Turkey
         * Turkey moves to inspect
         * Inspection
         * Inspection passes
         * Turkey moves off
         */
        TEST_METHOD(TestCControllerTurkeyBasicProcess)
        {
			// Create a test object
			CKitchenWnd kitchen;
			kitchen.Load();

			// Create a controller and install it
			auto controller = std::make_shared<CController>();
			kitchen.SetController(controller);



            for (int pass = 1; pass <= 3; pass++)
            {
                wstringstream str;
                str << L"BasicProcess pass: " << pass << endl;
                Logger::WriteMessage(str.str().c_str());

                // Ensure Next Turkey button is enabled
                Assert::IsTrue(kitchen.GetButtonNext()->IsEnabled());

                // Ensure Advance button is not enabled
                Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());

                //
                // Press the Next Turkey button
                //
                controller->OnNextPressed();

                // It takes one second for the Turkey to move in and stop.
                // Run the workshop for a bit more than that.
                kitchen.Update(1.1);

                // Is the Turkey created and in place at the wardrobe station (1)?
                auto turkey = kitchen.GetTurkey();
                Assert::IsTrue(turkey != nullptr);
                Assert::AreEqual(1, controller->GetTurkeyPosition(), 0.01);
                Assert::IsTrue(kitchen.GetButtonAdvance()->IsEnabled());

                //
                // Press the glasses button
                //
                controller->OnWardrobePressed(CKitchenController::Glasses);

                // 1 second for the glasses to fly to the turkey
                kitchen.Update(1.1);
                Assert::IsTrue(kitchen.GetButtonWardrobe()->IsEnabled());

                // Ensure glasses are there
                Assert::IsTrue(turkey->HasClothing(CKitchenController::Glasses));
                Assert::IsFalse(turkey->HasClothing(CKitchenController::Cardigan));

                //
                // Press the cardigan button
                //
                controller->OnWardrobePressed(CKitchenController::Cardigan);

                // 1 second for the cardigan to fly to the turkey
                kitchen.Update(1.1);
                Assert::IsTrue(kitchen.GetButtonWardrobe()->IsEnabled());

                // Ensure glasses are there
                Assert::IsTrue(turkey->HasClothing(CKitchenController::Glasses));
                Assert::IsTrue(turkey->HasClothing(CKitchenController::Cardigan));

                //
                // Advance
                //
                controller->OnAdvancePressed();

                // 3 seconds to proceed to inspection
                kitchen.Update(3.1);

                Assert::AreEqual(4, controller->GetTurkeyPosition(), 0.01);

                Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());
                Assert::IsTrue(kitchen.GetButtonInspect()->IsEnabled());
                Assert::AreEqual(wstring(L""), turkey->Saying());

                //
                // Inspect
                //
                controller->OnInspectPressed();

                kitchen.Update(0.5);
                Assert::AreEqual(wstring(L"Inspecting"), turkey->Saying());
                
                kitchen.Update(1.0);
                Assert::AreEqual(wstring(L"Passed"), turkey->Saying());

                // This should move the Turkey off the end and end the 
                // sequence. It should be ready for the next turkey.
                kitchen.Update(1.6);
            }


        }

        /**
         * This tests the turkey failing a wardrobe inspection.
         */
        TEST_METHOD(TestCControllerInspectionFailure)
        {
            // Create a test object
            CKitchenWnd kitchen;
            kitchen.Load();

            // Create a controller and install it
            auto controller = std::make_shared<CController>();
            kitchen.SetController(controller);

            //
            // Press the Next Turkey button
            //
            controller->OnNextPressed();

            // It takes one second for the Turkey to move in and stop.
            // Run the workshop for a bit more than that.
            kitchen.Update(1.1);

            // Is the Turkey created and in place at the wardrobe station (1)?
            auto turkey = kitchen.GetTurkey();
            Assert::IsTrue(turkey != nullptr);
            Assert::AreEqual(1, controller->GetTurkeyPosition(), 0.01);
            Assert::IsTrue(kitchen.GetButtonAdvance()->IsEnabled());

            //
            // Press the glasses button
            //
            controller->OnWardrobePressed(CKitchenController::Glasses);

            // 1 second for the glasses to fly to the turkey
            kitchen.Update(1.1);
            Assert::IsTrue(kitchen.GetButtonWardrobe()->IsEnabled());

            // Ensure glasses are there
            Assert::IsTrue(turkey->HasClothing(CKitchenController::Glasses));
            Assert::IsFalse(turkey->HasClothing(CKitchenController::Cardigan));

            //
            // Advance
            //
            controller->OnAdvancePressed();

            // 3 seconds to proceed to inspection
            kitchen.Update(3.1);

            Assert::AreEqual(4, controller->GetTurkeyPosition(), 0.01);

            Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());
            Assert::IsTrue(kitchen.GetButtonInspect()->IsEnabled());
            Assert::AreEqual(wstring(L""), turkey->Saying());

            //
            // Inspect
            //
            controller->OnInspectPressed();

            kitchen.Update(0.5);
            Assert::AreEqual(wstring(L"Inspecting"), turkey->Saying());

            // It should fail inspection, so 0.5 + 3 seconds later it should be
            // back at wardrobe
            kitchen.Update(3.6);

            Assert::AreEqual(wstring(L""), turkey->Saying());
            // Is the Turkey created and in place at the wardrobe station (1)?
            Assert::AreEqual(1, controller->GetTurkeyPosition(), 0.01);
            Assert::IsTrue(kitchen.GetButtonAdvance()->IsEnabled());
            Assert::IsFalse(kitchen.GetButtonInspect()->IsEnabled());
        }

        /**
         * This tests that the undressing feature works
         */
        TEST_METHOD(TestCControllerUndressing)
        {
            // Create a test object
            CKitchenWnd kitchen;
            kitchen.Load();

            // Create a controller and install it
            auto controller = std::make_shared<CController>();
            kitchen.SetController(controller);

            //
            // Press the Next Turkey button
            //
            controller->OnNextPressed();

            // It takes one second for the Turkey to move in and stop.
            // Run the workshop for a bit more than that.
            kitchen.Update(1.1);

            // Is the Turkey created and in place at the wardrobe station (1)?
            auto turkey = kitchen.GetTurkey();
            Assert::IsTrue(turkey != nullptr);
            Assert::AreEqual(1, controller->GetTurkeyPosition(), 0.01);
            Assert::IsTrue(kitchen.GetButtonAdvance()->IsEnabled());

            //
            // Press the glasses button
            //
            controller->OnWardrobePressed(CKitchenController::Glasses);

            // 1 second for the glasses to fly to the turkey
            kitchen.Update(1.1);
            Assert::IsTrue(kitchen.GetButtonWardrobe()->IsEnabled());

            // Ensure glasses are there
            Assert::IsTrue(turkey->HasClothing(CKitchenController::Glasses));
            Assert::IsFalse(turkey->HasClothing(CKitchenController::Cardigan));

            //
            // Press the cardigan button
            //
            controller->OnWardrobePressed(CKitchenController::Cardigan);

            // 1 second for the cardigan to fly to the turkey
            kitchen.Update(1.1);

            // Ensure both are there
            Assert::IsTrue(turkey->HasClothing(CKitchenController::Glasses));
            Assert::IsTrue(turkey->HasClothing(CKitchenController::Cardigan));

            //
            // Press the cardigan button
            //
            controller->OnWardrobePressed(CKitchenController::Cardigan);

            // 1 second for the cardigan to fly to the turkey
            kitchen.Update(1.1);

            // Ensure both are there
            Assert::IsTrue(turkey->HasClothing(CKitchenController::Glasses));
            Assert::IsFalse(turkey->HasClothing(CKitchenController::Cardigan));

            //
            // Press the glasses button
            //
            controller->OnWardrobePressed(CKitchenController::Glasses);

            // 1 second for the glasses to fly to the turkey
            kitchen.Update(1.1);

            // Ensure glasses are there
            Assert::IsFalse (turkey->HasClothing(CKitchenController::Glasses));
            Assert::IsFalse(turkey->HasClothing(CKitchenController::Cardigan));

            //
            // Press the cardigan button
            //
            controller->OnWardrobePressed(CKitchenController::Cardigan);

            // 1 second for the cardigan to fly to the turkey
            kitchen.Update(1.1);

            // Ensure both are there
            Assert::IsFalse(turkey->HasClothing(CKitchenController::Glasses));
            Assert::IsTrue(turkey->HasClothing(CKitchenController::Cardigan));

            //
            // Press the glasses button
            //
            controller->OnWardrobePressed(CKitchenController::Glasses);

            // 1 second for the glasses to fly to the turkey
            kitchen.Update(1.1);

            // Ensure glasses are there
            Assert::IsTrue(turkey->HasClothing(CKitchenController::Glasses));
            Assert::IsTrue(turkey->HasClothing(CKitchenController::Cardigan));

            Assert::IsFalse(kitchen.GetButtonNext()->IsEnabled());
            Assert::IsTrue(kitchen.GetButtonAdvance()->IsEnabled());

            //
            // Advance
            //
            controller->OnAdvancePressed();

            // 3 seconds to proceed to inspection
            kitchen.Update(3.1);

            Assert::AreEqual(4, controller->GetTurkeyPosition(), 0.01);

            Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());
            Assert::IsTrue(kitchen.GetButtonInspect()->IsEnabled());
            Assert::AreEqual(wstring(L""), turkey->Saying());

            //
            // Inspect
            //
            controller->OnInspectPressed();

            // 3 second should finish it all
            kitchen.Update(3.1);

            Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());
            Assert::IsFalse(kitchen.GetButtonInspect()->IsEnabled());
            Assert::IsTrue(kitchen.GetButtonNext()->IsEnabled());
        }

		/**
		 * This tests the entire process of a Turkey moving through 
		 * wardrobe, failing inspection, then adding the missing clothing items
         * and passing inspection.
		 */
		TEST_METHOD(TestCControllerEntireProcess)
		{
            // Create a test object
            CKitchenWnd kitchen;
            kitchen.Load();

            // Create a controller and install it
            auto controller = std::make_shared<CController>();
            kitchen.SetController(controller);

            for (int pass = 1; pass <= 3; pass++)
            {
                wstringstream str;
                str << L"EntireProcess pass: " << pass << endl;
                Logger::WriteMessage(str.str().c_str());

                // Ensure Next Turkey button is enabled
                Assert::IsTrue(kitchen.GetButtonNext()->IsEnabled());

                // Ensure Advance button is not enabled
                Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());

                //
                // Press the Next Turkey button
                //
                controller->OnNextPressed();

                // It takes one second for the Turkey to move in and stop.
                // Run the workshop for a bit more than that.
                kitchen.Update(1.1);

                // Is the Turkey created and in place at the wardrobe station (1)?
                auto turkey = kitchen.GetTurkey();
                Assert::IsTrue(turkey != nullptr);
                Assert::AreEqual(1, controller->GetTurkeyPosition(), 0.01);
                Assert::IsTrue(kitchen.GetButtonAdvance()->IsEnabled());

                //
                // Press the glasses button
                //
                controller->OnWardrobePressed(CKitchenController::Glasses);

                // 1 second for the glasses to fly to the turkey
                kitchen.Update(1.1);
                Assert::IsTrue(kitchen.GetButtonWardrobe()->IsEnabled());

                // Ensure glasses are there
                Assert::IsTrue(turkey->HasClothing(CKitchenController::Glasses));
                Assert::IsFalse(turkey->HasClothing(CKitchenController::Cardigan));

                //
                // Advance
                //
                controller->OnAdvancePressed();

                // 3 seconds to proceed to inspection
                kitchen.Update(3.1);

                Assert::AreEqual(4, controller->GetTurkeyPosition(), 0.01);

                Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());
                Assert::IsTrue(kitchen.GetButtonInspect()->IsEnabled());
                Assert::AreEqual(wstring(L""), turkey->Saying());

                //
                // Inspect
                //
                controller->OnInspectPressed();

                kitchen.Update(0.5);
                Assert::AreEqual(wstring(L"Inspecting"), turkey->Saying());

                // It should fail inspection, so 0.5 + 3 seconds later it should be
                // back at wardrobe
                kitchen.Update(3.6);

                Assert::AreEqual(wstring(L""), turkey->Saying());
                // Is the Turkey created and in place at the wardrobe station (1)?
                Assert::AreEqual(1, controller->GetTurkeyPosition(), 0.01);
                Assert::IsTrue(kitchen.GetButtonAdvance()->IsEnabled());
                Assert::IsFalse(kitchen.GetButtonInspect()->IsEnabled());

                //
                // Press the cardigan button
                //
                controller->OnWardrobePressed(CKitchenController::Cardigan);

                // 1 second for the cardigan to fly to the turkey
                kitchen.Update(1.1);
                Assert::IsTrue(kitchen.GetButtonWardrobe()->IsEnabled());

                // Ensure glasses are there
                Assert::IsTrue(turkey->HasClothing(CKitchenController::Glasses));
                Assert::IsTrue(turkey->HasClothing(CKitchenController::Cardigan));

                //
                // Advance
                //
                controller->OnAdvancePressed();

                // 3 seconds to proceed to inspection
                kitchen.Update(3.1);

                Assert::AreEqual(4, controller->GetTurkeyPosition(), 0.01);

                Assert::IsFalse(kitchen.GetButtonAdvance()->IsEnabled());
                Assert::IsTrue(kitchen.GetButtonInspect()->IsEnabled());
                Assert::AreEqual(wstring(L""), turkey->Saying());

                //
                // Inspect
                //
                controller->OnInspectPressed();

                kitchen.Update(0.5);
                Assert::AreEqual(wstring(L"Inspecting"), turkey->Saying());

                kitchen.Update(1.0);
                Assert::AreEqual(wstring(L"Passed"), turkey->Saying());

                // This should move the Turkey off the end and end the 
                // sequence. It should be ready for the next turkey.
                kitchen.Update(1.6);
            }

		}

	};
}