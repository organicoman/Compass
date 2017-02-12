// CameraWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "CameraWnd.h"



// CCameraWnd

IMPLEMENT_DYNCREATE(CCameraWnd, CFrameWndEx)

CCameraWnd::CCameraWnd()
{
	m_VideoCam = new CameraObj(this);
}

CCameraWnd::~CCameraWnd()
{
	delete m_VideoCam;
}

BEGIN_MESSAGE_MAP(CCameraWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL CCameraWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
	                    UINT nID, CCreateContext* pContext)
{
	// load stock cursor, brush, and icon for
	// my own window class
	if (lpszClassName == NULL)
	{
		try
		{
			lpszClassName = AfxRegisterWndClass(
				CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
				::LoadCursor(NULL, IDC_ARROW),
				(HBRUSH) ::GetStockObject(DKGRAY_BRUSH),
				::LoadIcon(NULL, IDI_APPLICATION));
		}
		catch (CResourceException* pEx)
		{
			AfxMessageBox(_T("Couldn't register class! (Already registered?)"));
			pEx->Delete();
		}
	}
	return CFrameWndEx::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd);
}

// CCameraWnd message handlers
int CCameraWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_toolbar.EnableLargeIcons(TRUE);

	if (!m_toolbar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_RIGHT | CBRS_SIZE_FIXED | CBRS_TOOLTIPS) ||
		!m_toolbar.LoadToolBar(IDW_CAMERA))
	{
		TRACE0("Failed to create and load Camera ToolBar\n");
		return-1;
	}
	m_toolbar.EnableTextLabels();
	CString labels[] = { _T("CAP"), _T("GRA"), _T("NOTE") };
	for (int i = 0; i<m_toolbar.GetCount(); i++)
	{
		CMFCToolBarButton* btn = m_toolbar.GetButton(i);

		btn->m_bText = TRUE;
		btn->m_bImage = FALSE;
		m_toolbar.SetButtonText(i, labels[i]);
	}


	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_toolbar);

	// This Window exists for the sole purpose of hosting the Video Stream,
	// so we cannot get that stream then return error, to cancel creation.

	// 1st Enumerate the Capture devices
	CameraObj::RETURNCODE ret = m_VideoCam->EnumerateCams(m_CamList);
	if (ret != CameraObj::RETURN_SUCCESS)
		return -1;

	// 2nd Connect to the camera (this should be selected in the SettingDlg)
	ret = m_VideoCam->ConnectToCam(m_CamList[1]);
	if (ret != CameraObj::RETURN_SUCCESS)
		return -1;
	
	// at this stage everything is setup. Running the Capture device done when the hWND handle exist
	return 0;
}

// This Handler is Mandatory for correct Video rendering (see MSDN VMR Windowless Mode. 
// URL=https://msdn.microsoft.com/en-us/library/windows/desktop/dd390962(v=vs.85).aspx
BOOL CCameraWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CCameraWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow &&(nStatus==SW_PARENTOPENING|| !nStatus))
	{
		// 3rd Run the Capture Process.
		CameraObj::RETURNCODE ret = m_VideoCam->Run();
		if (ret != CameraObj::RETURN_SUCCESS)
			TRACE("FAILURE: The Video Capture System could not be run!\n");
	}
	// TODO: Add your message handler code here
}
