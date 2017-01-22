// SpectrumWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "SpectrumWnd.h"


// CSpectrumWnd

IMPLEMENT_DYNCREATE(CSpectrumWnd, CFrameWndEx)

CSpectrumWnd::CSpectrumWnd()
{

}

CSpectrumWnd::~CSpectrumWnd()
{
}


BEGIN_MESSAGE_MAP(CSpectrumWnd, CFrameWndEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CSpectrumWnd message handlers


int CSpectrumWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_toolbar.EnableLargeIcons(TRUE);

	if (!m_toolbar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_RIGHT | CBRS_SIZE_FIXED | CBRS_TOOLTIPS) ||
		!m_toolbar.LoadToolBar(IDW_SPECTRUM))
	{
		TRACE0("Failed to create and load Spectrum ToolBar\n");
		return-1;
	}
	m_toolbar.EnableTextLabels();
	CString labels[] = { _T("ADD"), _T(""), _T("PTBL"), _T("ID"), _T("CLR"),_T("ANLZ"), _T(""), _T("+STD"), _T("CLB"),
						_T(""), _T("NOTE"), _T("OVRL"), _T("BACH") };
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


BOOL CSpectrumWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
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
				(HBRUSH) ::GetStockObject(BLACK_BRUSH),
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


void CSpectrumWnd::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd::PostNcDestroy();
	delete this;
}
