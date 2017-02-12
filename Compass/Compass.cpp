
// Compass.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Compass.h"
#include "CompassDlg.h"
#include "SettingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCompassApp

BEGIN_MESSAGE_MAP(CCompassApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCompassApp construction

CCompassApp::CCompassApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	CString CompassDirectory;

	BOOL err = CompassDirectory.GetEnvironmentVariable(_T("LOCALAPPDATA")); //on failure use the "\\Temp" folder

	if (!err)
		CompassDirectory.GetEnvironmentVariable(_T("TEMP"));

	CompassDirectory = CompassDirectory + CString(_T("\\Compass"));

	// Create this directory and save its path in AppDirectory
	BOOL bErr = CreateDirectory(CompassDirectory, 0);
	if (bErr == ERROR_PATH_NOT_FOUND)
	{
		TRACE("Failed to Create the Application Directory!\n");
		m_AppDirectory = _T(".");
	}
	else
	{
		m_AppDirectory = CompassDirectory;
	}
}


// The one and only CCompassApp object

CCompassApp theApp;


// CCompassApp initialization

BOOL CCompassApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("IXRF Systems Software Application"));

	// Welcome screen
	//CSettingDlg setup(m_AppDirectory);
	//INT_PTR nRet = setup.DoModal();
	//
	//if (nRet == IDOK)
	//{
	//	TRACE("Filled the Config File with the user settings\n ");
	//	setup.~CSettingDlg(); //yes calling to destructor, we don't need this object anymore.
	//}
	
	CCompassDlg dlg;
	m_pMainWnd = &dlg;

	//Start the Dialog Application
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
