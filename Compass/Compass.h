
// Compass.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Utility\XMLData.h"


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

public:
	CString m_AppDirectory;
	
};

extern CCompassApp theApp;