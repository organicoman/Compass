
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
	SetRegistryKey(_T("IXRFSystems"));
	
	// Setup the registry profile settings.
	m_bIsRegistryOK = _CreateProfile();

	// Welcome screen, Pass the m_bIsRegistryOK flag.
	CSettingDlg setupDlg(m_bIsRegistryOK);
	INT_PTR nRet = setupDlg.DoModal();
	
	if (nRet == IDOK)
	{
		// Transfer the User Selection to the App member structure 
		m_ActualSettings = setupDlg.m_CurrentSetting;
	}
	
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

BOOL CCompassApp::_CreateProfile()
{
#define SECTION_CAROUSEL _T("Carousel")
#define N_SLOTS_KEY _T("Number of Slots")
#define SECTION_CAMERA _T("Camera")
#define CAMERA_NAME_KEY _T("Camera Name")
#define CAMERA_ID_KEY _T("Camera Path")
#define EMPTY _T("N/A")

	BOOL ret;
	
	int nSlots = GetProfileInt(SECTION_CAROUSEL, N_SLOTS_KEY, -1/*as an error ret value*/);
	if (nSlots == -1)
	{
		ret = WriteProfileInt(SECTION_CAROUSEL, N_SLOTS_KEY, 8/*by default*/);
		if(!ret)
			return ret;
	}
	
	CString CameraName = GetProfileString(SECTION_CAMERA, CAMERA_NAME_KEY,EMPTY);
	if (CameraName == EMPTY)
	{
		ret = WriteProfileString(SECTION_CAMERA, CAMERA_NAME_KEY, EMPTY);
		if(!ret)
			return ret;
	}
	
	CString CameraID = GetProfileString(SECTION_CAMERA, CAMERA_ID_KEY, EMPTY);
	if (CameraID == EMPTY)
	{
		ret = WriteProfileString(SECTION_CAMERA, CAMERA_ID_KEY, EMPTY);
		if (!ret)
			return ret;
	}

	// Otherwise Store all the above valid data into the member Variable 
	m_ActualSettings = { nSlots, CameraName, CameraID };
	
	return TRUE;
}


int CCompassApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	// Upon Exit save the Current profile setting to the registry for later use.
#define SECTION_CAROUSEL _T("Carousel")
#define N_SLOTS_KEY _T("Number of Slots")
#define SECTION_CAMERA _T("Camera")
#define CAMERA_NAME_KEY _T("Camera Name")
#define CAMERA_ID_KEY _T("Camera Path")
#define EMPTY _T("N/A")
	
	BOOL ret1 = WriteProfileInt(SECTION_CAROUSEL, N_SLOTS_KEY, m_ActualSettings.m_nSlots);
	BOOL ret2 = WriteProfileString(SECTION_CAMERA, CAMERA_NAME_KEY, m_ActualSettings.m_strCurrentCamFriendlyName);
	BOOL ret3 = WriteProfileString(SECTION_CAMERA, CAMERA_ID_KEY, m_ActualSettings.m_strCurrentCamPath);
	if(!ret1 || !ret2 || !ret3)
		AfxMessageBox(_T("COMPASS FAILURE: Couldn't save the current user's settings!\n"), MB_OK);
	
	return CWinApp::ExitInstance();
}
