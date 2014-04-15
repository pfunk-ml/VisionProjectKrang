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

#include "testSimulation.h"

#include <wx/wx.h>
#include <GUI/Viewer.h>
#include <GUI/GUI.h>
#include <GUI/GRIPSlider.h>
#include <GUI/GRIPFrame.h>
#include <iostream>
#include <dart/dynamics/Skeleton.h>

#include <Tabs/AllTabs.h>
#include <GRIPApp.h>

#include "CameraCentralProcess.h"
#include "globalStuff/globalData.h"
#include "globalStuff/globalTransforms.h"
#include <ach.h>

ach_channel_t mSim_Channel;


/** Events */
enum testSimulationEvents {
    id_button_startConnection = 8345,
    id_button_querySim
};

/** Handler for events **/
BEGIN_EVENT_TABLE( testSimulation, wxPanel )
EVT_COMMAND ( wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, testSimulation::OnButton )
END_EVENT_TABLE()


// Class constructor for the tab: Each tab will be a subclass of GRIPTab
IMPLEMENT_DYNAMIC_CLASS( testSimulation, GRIPTab )


/**
 * @function testSimulation
 * @brief Constructor
 */
testSimulation::testSimulation( wxWindow *parent, const wxWindowID id,
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

    // SS3BoxS
    ss3BoxS->Add( new wxButton( this, id_button_startConnection, 
				wxT("Start Connection")), 0, wxALL, 1 );
    ss3BoxS->Add( new wxButton( this, id_button_querySim, 
				wxT("Query periodically")), 0, wxALL, 1 );

        
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

     // SET GLOBAL INFORMATION!!!!!	
      setGlobalData();
      setGlobalTransforms();
}



/**
 * @function GRIPEventSceneLoaded
 * @brief Functions that is called right after the world scene is loaded
 */
void testSimulation::GRIPEventSceneLoaded() {

}

/**
 * @function OnButton
 * @brief Handles button events
 */
void testSimulation::OnButton(wxCommandEvent & _evt) {
  
    int slnum = _evt.GetId();
    
    switch( slnum ) {
      
      // Start connection && SET HARD CODED VALUES
    case id_button_startConnection : {

      // Start our own channel to read
      int r;
      r = ach_open( &mSim_Channel, PERCEPTION_CHANNEL, NULL );
      assert( ACH_OK == r );
      
      std::cout << "Opened successfully channel "<< PERCEPTION_CHANNEL << std::endl;
      
      // Set cameras  
      Eigen::Isometry3d Transf;
      std::cout << "Setting cam 0 "<< std::endl;
      Transf.matrix() = gTworld_cam[0];
      mWorld->getSkeleton("cam0")->setConfig( dart::math::logMap( Transf ) ); 
      
      std::cout << "Setting cam 1 "<< std::endl;
      Transf.matrix() = gTworld_cam[1];
      mWorld->getSkeleton("cam1")->setConfig( dart::math::logMap( Transf ) ); 
      
      std::cout << "Setting cam 2 "<< std::endl;
      Transf.matrix() = gTworld_cam[2];
      mWorld->getSkeleton("cam2")->setConfig( dart::math::logMap( Transf ) ); 
      
      std::cout << "Setting cam 3 "<< std::endl;
      Transf.matrix() = gTworld_cam[3];
      mWorld->getSkeleton("cam3")->setConfig( dart::math::logMap( Transf ) ); 
      
      std::cout <<"SET CAMERAS CORRECTLY" << std::endl;
      
      
    } break;
      
      
      // Query sim
    case id_button_querySim : {
      
      /*
      Planning_output po[NUM_OBJECTS];
      size_t fs; int r; 
      
      r = ach_get( &mSim_Channel, 
		   po,
		   sizeof(po),
		   &fs,
		   NULL, ACH_O_LAST );
      assert( (ACH_OK == r || ACH_MISSED_FRAME == r ) &&
	      sizeof( po ) == fs );
      
      std::cout << "Reading objects less the first (origin) one" << std::endl;
      for( int i = 0; i < NUM_OBJECTS; ++i ) {
	
	Eigen::Matrix4d Tf = Eigen::Matrix4d::Identity();
	if( po[i].visible == -1 ) { 
	  std::cout << "Object"<< i << " not visible" << std::endl;
	}
	else
	  {
	    std::cout << "Object "<< i <<" is visible" << std::endl;
	    
	    for( int j = 0; j < 3; ++j ) {
			for( int k = 0; k < 4; ++k ) {
			  Tf(j,k) = po[i].Tworld_marker[j][k];
			}
	    }
	    std::cout << Tf << std::endl;
	  }
	
	switch( i ) {
	case 0 : mWorld->getSkeleton("marker0")->setConfig( dart::math::logMap( Eigen::Isometry3d(Tf) ) ); break;
	case 1 : mWorld->getSkeleton("marker1")->setConfig( dart::math::logMap( Eigen::Isometry3d(Tf) ) ); break;
	case 2 : mWorld->getSkeleton("marker2")->setConfig( dart::math::logMap( Eigen::Isometry3d(Tf) ) ); break;
	case 3 : mWorld->getSkeleton("marker3")->setConfig( dart::math::logMap( Eigen::Isometry3d(Tf) ) ); break;

	}
	
      }
      
      std::cout << "Finished setting objects "<<std::endl;
      */

      double po[NUM_OBJECTS][4];
      size_t fs; int r; 
      
      r = ach_get( &mSim_Channel, 
		   po,
		   sizeof(po),
		   &fs,
		   NULL, ACH_O_LAST );
	std::cout << "Size of message (po) : " << sizeof(po) << " and fs: "<< fs << std::endl;
      assert( (ACH_OK == r || ACH_MISSED_FRAME == r ) &&
	      sizeof( po ) == fs );

      
      std::cout << "Reading objects global location" << std::endl;
      for( int i = 0; i < NUM_OBJECTS; ++i ) {
	
	Eigen::Matrix4d Tf = Eigen::Matrix4d::Identity();
	if( po[i][0] == -1 ) { 
	  std::cout << "Object"<< i << " NOT visible" << std::endl;
	  std::cout << std::endl;
	}
	else {
	  std::cout << "Object "<< i <<" is visible" << std::endl;
	  
	  double x, y, theta;
	  x = po[i][1];
	  y = po[i][2];
	  theta = po[i][3];	
	  
	  Eigen::Matrix3d rotation;
	  rotation = Eigen::AngleAxisd(theta, Eigen::Vector3d::UnitZ());
	  Tf.block(0,0,3,3) = rotation;
	  Tf.block(0,3,3,1) = Eigen::Vector3d(x, y, 0);
	  
	  std::cout << Tf << std::endl;
	}
	
	switch( i ) {
	case 0 : mWorld->getSkeleton("robot")->setConfig( dart::math::logMap( Eigen::Isometry3d(Tf) ) ); break;
	case 1 : mWorld->getSkeleton("table")->setConfig( dart::math::logMap( Eigen::Isometry3d(Tf) ) ); break;
	case 2 : mWorld->getSkeleton("chair")->setConfig( dart::math::logMap( Eigen::Isometry3d(Tf) ) ); break;
	}
	
      }
      
      std::cout << "Finished setting objects "<<std::endl;
      
      
    } break; // End querySim Button case
      
    } // End switch button
}



/**
 * @function GRIPEventSimulationBeforeTimeStep
 * @brief Before each sim step we must set the internal forces 
 */
void testSimulation::GRIPEventSimulationBeforeTimestep() {
}

/**
 * @function GRIPEventSimulationAfterTimeStep
 * @brief
 */
void testSimulation::GRIPEventSimulationAfterTimestep() {
}

/**
 * @function GRIPEventSimulationStart
 * @brief
 */
void testSimulation::GRIPEventSimulationStart() {

}



// This function is called when an object is selected in the Tree View or other
// global changes to the GRIP world. Use this to capture events from outside the tab.
void testSimulation::GRIPStateChange() {
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


