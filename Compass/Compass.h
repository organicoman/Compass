
// Compass.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Utility/VideoCapture.h"

// Utility Structure to store Settings Data, Use this struct to transport data between your code's sections.(A mule!)
struct sProfile
{
	// Carousel number of Slots.
	int m_nSlots;
	
	// a Friendly name that can be used in UI controls (like a combo box).
	CString m_strCurrentCamFriendlyName;

	// The ID that differentiates between two cameras with the same FriendlyName(Very Important)
	CString m_strCurrentCamPath;
};


// CCompassApp:
// See Compass.cpp for the implementation of this class
//

class CCompassApp : public CWinApp
{
public:
	CCompassApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
private:
	BOOL _CreateProfile();

public:
	// Stores this instance user settings
	sProfile m_ActualSettings;

	// if TRUE, then Registry Profile has been setup.
	// (the registry keys could contain non useful data like the value "N/A". Always Check the Keys!)
	BOOL m_bIsRegistryOK;
	virtual int ExitInstance();
};

extern CCompassApp theApp;