// CameraWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "CameraWnd.h"


// CCameraWnd

IMPLEMENT_DYNCREATE(CCameraWnd, CFrameWndEx)

CCameraWnd::CCameraWnd():m_ViewMode(MODE::WINDOW_MODE)
{

}

CCameraWnd::~CCameraWnd()
{
}


BEGIN_MESSAGE_MAP(CCameraWnd, CFrameWndEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CCameraWnd message handlers


BOOL CCameraWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
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


int CCameraWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_toolbar.EnableLargeIcons(TRUE);

	if (!m_toolbar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_RIGHT | CBRS_SIZE_FIXED | CBRS_TOOLTIPS) ||
		!m_toolbar.LoadToolBar(m_ViewMode==MODE::WINDOW_MODE? IDW_CAMERA: IDW_CAMERA_VIEW))
	{
		TRACE0("Failed to create and load Camera ToolBar\n");
		return-1;
	}
	m_toolbar.EnableTextLabels();
	CString labels[] = { _T("CAP"), _T("GRA"),_T("NOTE") };
	for (int i = 0; i<m_toolbar.GetCount(); i++)
	{
		CMFCToolBarButton* btn = m_toolbar.GetButton(i);

		btn->m_bText = TRUE;
		btn->m_bImage = FALSE;
		m_toolbar.SetButtonText(i, labels[i]);
	}


	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_toolbar);

	return 0;
}

void CCameraWnd::SetMode(MODE enmMode)
{
	m_ViewMode = enmMode;
}

// Gets the Camera View mode (i.e is it a Window or a View in a Window)
CCameraWnd::MODE CCameraWnd::GetMode()
{
	return m_ViewMode;
}
