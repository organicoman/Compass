// CarouselWnd.cpp : implementation file
//
#pragma once

#include "stdafx.h"
#include "Compass.h"
#include "CarouselWnd.h"

// CCarouselWnd

IMPLEMENT_DYNCREATE(CCarouselWnd, CFrameWndEx)

CCarouselWnd::CCarouselWnd()
{
}

CCarouselWnd::~CCarouselWnd()
{
}

BEGIN_MESSAGE_MAP(CCarouselWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(IDC_BUTTON_VIEW, &CCarouselWnd::OnViewButtonClicked)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CCarouselWnd message handlers

int CCarouselWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_toolbar.EnableLargeIcons(TRUE);

	if (!m_toolbar.Create(this,WS_CHILD|WS_VISIBLE|CBRS_RIGHT|CBRS_SIZE_FIXED|CBRS_TOOLTIPS)||
		!m_toolbar.LoadToolBar(IDW_CAROUSEL))
	{
		TRACE0("Failed to create and load Carousel ToolBar\n");
		return-1;
	}
	m_toolbar.EnableTextLabels();
	CString labels[] = { _T("ADD"), _T("CAPT"), _T("GRAT"), _T(""),_T("CAM") };
	for (int i=0; i<m_toolbar.GetCount();i++)
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

BOOL CCarouselWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// load stock cursor, brush, and icon for
	// my own window class
	if (lpszClassName == NULL)
	{
		try
		{
			lpszClassName = AfxRegisterWndClass(
				CS_VREDRAW | CS_HREDRAW |CS_DBLCLKS,
				::LoadCursor(NULL, IDC_ARROW),
				(HBRUSH) ::GetStockObject(LTGRAY_BRUSH),
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

void CCarouselWnd::OnViewButtonClicked()
{
	if (fCamOnOff == FALSE)
	{
		CRect rect;
		GetClientRect(&rect);
		int x = rect.Width() / 2;
		int y = 5;
		rect.left = x - 50;
		rect.top = y;
		rect.right = rect.right - 50;
		rect.bottom = rect.Height() / 2 - 5;
		
		m_Camera = new CCameraWnd;
		ASSERT(m_Camera);
		
		if (!m_Camera->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rect, this, 1234)) 
		{
			TRACE0("Failure: error while creating the camera view window\n");
		}
		fCamOnOff = TRUE;
	}
	else
	{
		m_Camera->DestroyWindow();
		m_Camera = nullptr;
		fCamOnOff = FALSE;
	}
	
}


void CCarouselWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CFrameWndEx::OnPaint() for painting messages
	CRect rect;
	GetClientRect(&rect);
	int x = rect.Width() / 2;
	int y = rect.Height() / 2;
	int radius = x < y ? (x - 20) : (y - 20);
	dc.Ellipse(x - radius, y - radius, x + radius, y + radius);
	CBrush color{ RGB(100,200,100) };
	CBrush* oldColor = dc.SelectObject(&color);
	int sRad = radius / 4;
	for (int i = 0; i < 8; i++)
	{
		const double pi = acos(-1.0);
		double angle = pi * i /4.0 ;
		
		double sx = x + (radius - (sRad + 5))*cos(angle);
		double sy = y + (radius - (sRad + 5))*sin(angle);
		dc.Ellipse(sx - sRad, sy - sRad, sx + sRad, sy + sRad);
	}
	dc.SelectObject(oldColor);
}

void CCarouselWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	if (m_Camera != nullptr)
	{
		CRect rect;
		GetClientRect(&rect);
		int x = rect.Width() / 2;
		int y = 5;
		rect.left = x - 50;
		rect.top = y;
		rect.right = rect.right - 50;
		rect.bottom = rect.Height() / 2 - 5;

		m_Camera->MoveWindow(&rect);
	}
}
