/*
 * Copyright (c) 2013, Georgia Tech Research Corporation
 * 
 * Humanoid Robotics Lab      Georgia Institute of Technology
 * Director: Mike Stilman     http://www.golems.org
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the Georgia Tech Research Corporation nor
 *       the names of its contributors may be used to endorse or
 *       promote products derived from this software without specific
 *       prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GEORGIA TECH RESEARCH CORPORATION ''AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GEORGIA
 * TECH RESEARCH CORPORATION BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "cameraController.h"

#include <wx/wx.h>
#include <GUI/Viewer.h>
#include <GUI/GUI.h>
#include <GUI/GRIPSlider.h>
#include <GUI/GRIPFrame.h>
#include <iostream>
#include <dart/dynamics/Skeleton.h>

#include <Tabs/AllTabs.h>
#include <GRIPApp.h>

#include "../globalData.h"
#include "../Object.h"
#include <ach.h>

ObjectData_t mObjects[NUM_OBJECTS];
std::vector<ach_channel_t> mCam_Channels;


/** Events */
enum cameraControllerEvents {
    id_button_openCam0 = 8345,
    id_button_openCam1,
    id_button_openCam2,
    id_button_openCam3,
    id_button_startConnection,
    id_button_getInfo
};

/** Handler for events **/
BEGIN_EVENT_TABLE( cameraController, wxPanel )
EVT_COMMAND ( wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, cameraController::OnButton )
END_EVENT_TABLE()


// Class constructor for the tab: Each tab will be a subclass of GRIPTab
IMPLEMENT_DYNAMIC_CLASS( cameraController, GRIPTab )


/**
 * @function cameraController
 * @brief Constructor
 */
cameraController::cameraController( wxWindow *parent, const wxWindowID id,
				    const wxPoint& pos, const wxSize& size, long style ) :
GRIPTab( parent, id, pos, size, style ) {
    sizerFull = new wxBoxSizer(wxHORIZONTAL);
    
    // Create Static boxes (outline of your Tab)
    wxStaticBox* ss1Box = new wxStaticBox(this, -1, wxT("Empty"));
    wxStaticBox* ss2Box = new wxStaticBox(this, -1, wxT("Start cameras"));
    wxStaticBox* ss3Box = new wxStaticBox(this, -1, wxT("Central process"));
    
    // Create sizers for these static boxes
    wxStaticBoxSizer* ss1BoxS = new wxStaticBoxSizer(ss1Box, wxVERTICAL);
    wxStaticBoxSizer* ss2BoxS = new wxStaticBoxSizer(ss2Box, wxVERTICAL);
    wxStaticBoxSizer* ss3BoxS = new wxStaticBoxSizer(ss3Box, wxVERTICAL);
    
    

    // SS1BoxS
    //ss1BoxS->Add( mSlider_A, 0, wxALL, 1 );

    // SS2BoxS
    ss2BoxS->Add( new wxButton( this, id_button_openCam0, 
				wxT("Open Camera 0")), 0, wxALL, 1 );
    ss2BoxS->Add( new wxButton( this, id_button_openCam1, 
				wxT("Open Camera 1")), 0, wxALL, 1 );
    ss2BoxS->Add( new wxButton( this, id_button_openCam2, 
				wxT("Open Camera 2")), 0, wxALL, 1 );
    ss2BoxS->Add( new wxButton( this, id_button_openCam3, 
				wxT("Open Camera 3")), 0, wxALL, 1 );

    // SS3BoxS
    ss3BoxS->Add( new wxButton( this, id_button_startConnection, 
				wxT("Start Connection")), 0, wxALL, 1 );
    ss3BoxS->Add( new wxButton( this, id_button_getInfo, 
				wxT("Get information")), 0, wxALL, 1 );

        
    // Add the boxes to their respective sizers
    sizerFull->Add(ss1BoxS, 1, wxEXPAND | wxALL, 6);
    sizerFull->Add(ss2BoxS, 1, wxEXPAND | wxALL, 6);
    sizerFull->Add(ss3BoxS, 1, wxEXPAND | wxALL, 6);
    
    SetSizer(sizerFull);
    
    // Additional settings
    mCurrentFrame = 0;  
    viewer->backColor = Vector3d(1,1,1);
    viewer->gridColor = Vector3d(.8,.8,1);
    viewer->setClearColor();
    viewer->DrawGLScene();  
}


/**< Fork and exec a subprocess running the camera program */
int cameraController::spawnCamera( char* _camProgram,
				   char** _argList ) {
    pid_t child_pid;
    
    /** Duplicate this process */
    child_pid = fork();
    
    /**< Parent process */
    if( child_pid != 0 ) {
	return child_pid;
    }

    /**< Child process */
    else {
	execvp( _camProgram, _argList);
	/**< execvp only comes back if an error*/
	fprintf( stderr, "An error occurred in execvp %s calling the program \n", _camProgram );
	abort();
    }
}


/**
 * @function GRIPEventSceneLoaded
 * @brief Functions that is called right after the world scene is loaded
 */
void cameraController::GRIPEventSceneLoaded() {

}

/**
 * @function OnButton
 * @brief Handles button events
 */
void cameraController::OnButton(wxCommandEvent & _evt) {
  
    int slnum = _evt.GetId();
    
    switch( slnum ) {
	
	// Open Cam 0
    case id_button_openCam0 : {
	char* arg_list[] = {"camProcess", "0", NULL };
	spawnCamera( "./camProcess", arg_list );
	std::cout << "Done calling cam 0"<< std::endl;
    } break;

	// Open Cam 1
    case id_button_openCam1 : {
	char* arg_list[] = {"camProcess", "1", NULL };
	spawnCamera( "./camProcess", arg_list );
	std::cout << "Done calling cam 1"<< std::endl;
    } break;

	// Open Cam 2
    case id_button_openCam2 : {
	std::cout << "Not implemented yet" << std::endl;	
    } break;

	// Open Cam 3
    case id_button_openCam3 : {
	std::cout << "Not implemented yet" << std::endl;
    } break;

	// Start connection
    case id_button_startConnection : {
	std::cout << "Attempt to start connection" << std::endl;
	int r;
	int counter;
	std::string name;

	counter = 0;
	for( int i = 0; i < NUM_OBJECTS; ++i ) {

	    switch(i) {
	    case 0 : name = CAM0_CHANNEL; break;
	    case 1 : name = CAM1_CHANNEL; break;
	    case 2 : name = CAM2_CHANNEL; break;
	    case 3 : name = CAM3_CHANNEL; break;
	    };
	    
	    ach_channel_t chan;
	    r = ach_open( &chan, name.c_str(), NULL );
	    if( ACH_OK == r ) {
		mCam_Channels.push_back( chan );
		counter++; 
		std::cout << "Opened successfully channel "<<name << std::endl;
	    } else {
		std::cout << "Channel "<< name << " not opened "<< std::endl;
	    }	    
	}



    } break;

	// Get info
    case id_button_getInfo : {

	// Get objects location
	size_t fs;
	int r; 
	for( int i = 0; i < mCam_Channels.size(); ++i ) {
	    r = ach_get( &mCam_Channels[i],
			 mObjects, sizeof( mObjects ),
			 &fs,
			 NULL, ACH_O_WAIT );
	    assert( (ACH_OK == r || ACH_MISSED_FRAME == r ) &&
		    sizeof(mObjects) == fs );
     
	    std::cout << "Received channel :"<< i<< " \n"<< std::endl;
	}

	for( int i = 0; i < NUM_OBJECTS; ++i ) {
	    
	    if( mObjects[i].visible == -1 ) { continue; }

	    Eigen::Isometry3d Tf = Eigen::Isometry3d::Identity();
	    for( int j = 0; j <3; ++j ) {
		for( int k = 0; k < 4; ++k ) {
		    Tf.matrix()(j,k) = mObjects[i].trans[j][k];
		}
	    }
	    Tf.translation() = Tf.translation() / 1000.0;	    

	    // Set cube is hiro
	    if( i == 0 ) {
		if( mObjects[i].visible >= 0 ) {
		    mWorld->getSkeleton("origin")->setConfig( dart::math::logMap(Tf) );
		}
	    }
	    
	    
	    // Set chair is kanji
	    if( i == 1 ) {
		if( mObjects[i].visible >= 0 ) {
		    mWorld->getSkeleton("chair1")->setConfig( dart::math::logMap(Tf) );
		}
	    }

	    // Set table is sample1
	    if( i == 2 ) {
		if( mObjects[i].visible >= 0 ) {
		    mWorld->getSkeleton("table1")->setConfig( dart::math::logMap(Tf) );
		}
	    }


	}

	
    } break;

	
    }
}



/**
 * @function GRIPEventSimulationBeforeTimeStep
 * @brief Before each sim step we must set the internal forces 
 */
void cameraController::GRIPEventSimulationBeforeTimestep() {
}

/**
 * @function GRIPEventSimulationAfterTimeStep
 * @brief
 */
void cameraController::GRIPEventSimulationAfterTimestep() {
}

/**
 * @function GRIPEventSimulationStart
 * @brief
 */
void cameraController::GRIPEventSimulationStart() {

}



// This function is called when an object is selected in the Tree View or other
// global changes to the GRIP world. Use this to capture events from outside the tab.
void cameraController::GRIPStateChange() {
  if(selectedTreeNode==NULL){
    return;
  }
  
  string statusBuf;
  string buf, buf2;
  switch (selectedTreeNode->dType) {
  case Return_Type_Robot:
    // Not much
    break;
  case Return_Type_Node:
    // Not much
    break;
  default:
    fprintf(stderr, "someone else's problem.");
    assert(0);
    exit(1);
  }
  //frame->SetStatusText(wxString(statusBuf.c_str(), wxConvUTF8));
  //sizerFull->Layout();
}


