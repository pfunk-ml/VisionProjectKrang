/**
 * @file testSimulationApp.h
 * @brief Creates application for berenson09
 * @author A. Huaman Q.
 */
#include "GRIPApp.h"
#include "testSimulation.h"

extern wxNotebook* tabView;

class testSimulationApp : public GRIPApp {
	virtual void AddTabs() {
		tabView->AddPage(new testSimulation(tabView), 
				 wxT("testSimulation"));
	}
};

IMPLEMENT_APP(testSimulationApp)
