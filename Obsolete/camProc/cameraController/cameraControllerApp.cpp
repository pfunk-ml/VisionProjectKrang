/**
 * @file cameraControllerApp.h
 * @brief Creates application for berenson09
 * @author A. Huaman Q.
 */
#include "GRIPApp.h"
#include "cameraController.h"

extern wxNotebook* tabView;

class cameraControllerApp : public GRIPApp {
	virtual void AddTabs() {
		tabView->AddPage(new cameraController(tabView), 
				 wxT("cameraController"));
	}
};

IMPLEMENT_APP(cameraControllerApp)
