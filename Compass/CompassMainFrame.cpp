// AtlasMainFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "CompassMainFrame.h"

// CCompassMainFrame

CCompassMainFrame::CCompassMainFrame():
	m_enmCurrentWnd(WND_CAROUSEL)
{
}

CCompassMainFrame::~CCompassMainFrame()
{
}


BEGIN_MESSAGE_MAP(CCompassMainFrame, CFrameWnd)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCompassMainFrame message handlers


BOOL CCompassMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;
	
	return CFrameWnd::PreCreateWindow(cs);
}

BOOL CCompassMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext))
	{
		TRACE0("Failed to Create Main-Frame Window\n");
		return -1;
	}
	//Capture the internal DynamicLayout Manager
	EnableDynamicLayout();
	CMFCDynamicLayout* layout = GetDynamicLayout();
	ASSERT(layout);
	if (!layout)
	{
		TRACE0("Failed to catch the DynamicLayout pointer\n");
		return FALSE;
	}
	if (layout->GetHostWnd() == NULL)
	{
		if (!layout->Create(this)) // and attach it to this Window
		{
			TRACE0("Failed to populate the DynamicLayout\n");
			return FALSE;
		}
	}
	
	// make sure the DynamicLayout items list is empty 
	ASSERT(layout->IsEmpty() == TRUE);

	//at first creation the CarouselWnd is the first screen
	return SetupCarouselWnd();
}

// Switch between the different screens (Camera, Carousel, Spectrum...)
BOOL CCompassMainFrame::SwitchWnd(CURRENTWND enIDwnd)
{
	if (enIDwnd == m_enmCurrentWnd)
	{
		UpdateWindow();
		return TRUE;
	}

	//Create the selected window, set it as the actual frame then destory the old one, and display this one
	
	switch (enIDwnd)
	{
	case WND_CAMERA:
		if (m_wndCamera == nullptr) //if pointer is null then create the wnd
		{
			if (SetupCameraWnd() == FALSE)
			{
				TRACE0("Error creating the Camera Window\n");
				return FALSE;
			}
			KillCurrentWnd(m_enmCurrentWnd);//Breakpoint here if you think that this function causes flickering
		}
		m_enmCurrentWnd = enIDwnd;
		break;
	case WND_SPECTRUM:
		if (m_wndSpectrum == nullptr) 
		{
			if (SetupSpectrumWnd() == FALSE)
			{
				TRACE0("Error creating the Spectrum Window\n");
				return FALSE;
			}
			KillCurrentWnd(m_enmCurrentWnd);
		}
		m_enmCurrentWnd = enIDwnd;
		break;
	case WND_CAROUSEL:
		if (m_wndCarousel == nullptr) 
		{
			if (SetupCarouselWnd() == FALSE)
			{
				TRACE0("Error creating the Carousel Window\n");
				return FALSE;
			}
			KillCurrentWnd(m_enmCurrentWnd);
		}
		m_enmCurrentWnd = enIDwnd;
		break;
	case WND_PROPERTY:
		if (m_wndProperty == nullptr)
		{
			if (SetupPropertyWnd() == FALSE)
			{
				TRACE0("Error creating the Property Window\n");
				return FALSE;
			}
			KillCurrentWnd(m_enmCurrentWnd);
		}
		m_enmCurrentWnd = enIDwnd;
		break;
	}

	Invalidate();// invalidate all the client area of the mainframe
	
	UpdateWindow();// then repaint it.
	
	return TRUE;
}

// Create and Populate the Carousel Wnd then Add it to the DynamicLayoutManager
BOOL CCompassMainFrame::SetupCarouselWnd()
{
	CRect clientRect;
	GetClientRect(&clientRect);
	
	m_wndCarousel = new CCarouselWnd;
	ASSERT(m_wndCarousel);
	if (!m_wndCarousel)
	{
		TRACE0("Failed to allocate memory for CarouselWnd\n");
		return FALSE;
	}

	if (!m_wndCarousel->Create(NULL, _T("Carousel"), WS_CHILD | WS_VISIBLE | FWS_ADDTOTITLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, clientRect, this,
		IDW_CAROUSEL, NULL/*to add the view context*/))
	{
		TRACE0("Error: Failed to create the Child Window -Carousel\n");
		return FALSE;
	}
	
	CMFCDynamicLayout* layout = GetDynamicLayout();
	
	CMFCDynamicLayout::MoveSettings move;
	CMFCDynamicLayout::SizeSettings size;
	size.m_nXRatio = 100;
	size.m_nYRatio = 100;
	layout->AddItem(m_wndCarousel->GetSafeHwnd(), move, size);

	m_wndCarousel->SetActiveWindow();
	
	return TRUE;
}

// Create and populate the Camera Wnd
BOOL CCompassMainFrame::SetupCameraWnd()
{
	CRect clientRect;
	GetClientRect(&clientRect);
	
	m_wndCamera = new CCameraWnd;
	ASSERT(m_wndCamera);
	if (!m_wndCamera)
	{
		TRACE0("Failed to allocate memory for CameraWnd\n");
		return FALSE;
	}
	
	if (!m_wndCamera->Create(NULL, _T("Camera"), WS_CHILD | WS_VISIBLE | FWS_ADDTOTITLE | WS_CLIPCHILDREN, clientRect, this,
		IDW_CAMERA, NULL/*to add the view context*/))
	{
		TRACE0("Error: Failed to create the Child Window -Camera\n");
		return FALSE;
	}
	
	CMFCDynamicLayout *layout = GetDynamicLayout();

	CMFCDynamicLayout::MoveSettings move;
	CMFCDynamicLayout::SizeSettings size;
	size.m_nXRatio = 100;
	size.m_nYRatio = 100;
	layout->AddItem(m_wndCamera->GetSafeHwnd(), move, size);

	m_wndCamera->SetActiveWindow();
	
	return TRUE;
}

// Create and populate the the Spectrum Wnd then Add it to the DynamicLayout Manger
BOOL CCompassMainFrame::SetupSpectrumWnd()
{
	CRect clientRect;
	GetClientRect(&clientRect);
	
	m_wndSpectrum = new CSpectrumWnd;
	ASSERT(m_wndSpectrum);
	if (!m_wndSpectrum)
	{
		TRACE0("Failed to allocate memory for SpectrumWnd\n");
		return FALSE;
	}

	if (!m_wndSpectrum->Create(NULL, _T("Spectrum"), WS_CHILD | WS_VISIBLE | FWS_ADDTOTITLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, clientRect, this,
		IDW_SPECTRUM, NULL/*to add the view context*/))
	{
		TRACE0("Error: Failed to create the Child Window -Spectrum\n");
		return FALSE;
	}

	CMFCDynamicLayout* layout = GetDynamicLayout();
		
	CMFCDynamicLayout::MoveSettings move;
	CMFCDynamicLayout::SizeSettings size;
	size.m_nXRatio = 100;
	size.m_nYRatio = 100;
	layout->AddItem(m_wndSpectrum->GetSafeHwnd(), move, size);

	m_wndSpectrum->SetActiveWindow();
	
	return TRUE;
}

// Create and populate the the Property Wnd then Add it to the DynamicLayout Manger
BOOL CCompassMainFrame::SetupPropertyWnd()
{
	CRect clientRect;
	GetClientRect(&clientRect);
	
	m_wndProperty = new CPropertyWnd(_T("Properties"));
	ASSERT(m_wndProperty);
	if (!m_wndProperty)
	{
		TRACE0("Failed to allocate memory for PropertyWnd\n");
		return FALSE;
	}
	
	if (!m_wndProperty->Create(this, WS_CHILD|WS_VISIBLE, WS_CLIPCHILDREN))
	{
		TRACE0("Error: Failed to create the Child Window -Property\n");
		return FALSE;
	}
	//we keep the original height of the propertysheet window.
	CRect orgRect;
	m_wndProperty->GetWindowRect(&orgRect);
	m_wndProperty->MoveWindow(0, 0, clientRect.Width(), orgRect.Height(), 1 /*repaint window*/);
	
	CMFCDynamicLayout* layout = GetDynamicLayout();

	CMFCDynamicLayout::MoveSettings move;
	CMFCDynamicLayout::SizeSettings size;
	size.m_nXRatio = 100;//scale the propertysheet window horizontaly with the parent window
		
	layout->AddItem(m_wndProperty->GetSafeHwnd(), move, size);

	m_wndProperty->AdjustPagesLayout();// methode to adjust page layout
		
	//Shows and sets the vertical scrollbarCtrl for this screen only
	ShowScrollBar(SB_VERT);
	SCROLLINFO sbInfo;
	if (GetScrollInfo(SB_VERT, &sbInfo))
	{
		sbInfo.cbSize = sizeof(SCROLLINFO);
		sbInfo.fMask = SIF_PAGE | SIF_RANGE;
		sbInfo.nPage = clientRect.Height(); //page = height of mainframe
		sbInfo.nMin = 0;
		sbInfo.nMax = orgRect.Height(); // total height = height of the propertysheet window.
		
		SetScrollInfo(SB_VERT, &sbInfo);
	}
	m_wndProperty->SetActiveWindow();
		
	return TRUE;
}

// Deactivate then destroy the actual child window, to free the pointer resource.
void CCompassMainFrame::KillCurrentWnd(CURRENTWND enmIDwnd)
{
	// depending on the parameter; ensure that we are not destoying an already destroyed hWnd.
	switch (enmIDwnd)
	{
	case WND_CAROUSEL:
		ASSERT_VALID(m_wndCarousel);
		if (m_wndCarousel != nullptr)
		{
			m_wndCarousel->DestroyWindow();
			m_wndCarousel = nullptr;
		}
		break;
	case WND_CAMERA:
		ASSERT_VALID(m_wndCamera);
		if (m_wndCamera != nullptr)
		{
			m_wndCamera->DestroyWindow();
			m_wndCamera = nullptr;
		}
		break;
	case WND_SPECTRUM:
		ASSERT_VALID(m_wndSpectrum);
		if (m_wndSpectrum != nullptr)
		{
			m_wndSpectrum->DestroyWindow();
			m_wndSpectrum = nullptr;
		}
		break;
	case WND_PROPERTY:
		ASSERT_VALID(m_wndProperty);
		if (m_wndProperty != nullptr)
		{
			ShowScrollBar(SB_VERT, FALSE); //Hides vertical scrollbarctrl after destruction
			m_wndProperty->DestroyWindow();
			m_wndProperty = nullptr;
		}
		break;
	}
}

// Handles the Vscroll control bar in the presence of the Property Screen
void CCompassMainFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//test if the current screen is not the Property Window
	if (m_enmCurrentWnd != WND_PROPERTY) return;

	//Get the Current ClientArea dimensions
	CRect rect;
	m_wndProperty->GetWindowRect(&rect);
	ScreenToClient(&rect);

	SCROLLINFO sbInfo;
	GetScrollInfo(SB_VERT, &sbInfo);
	int length = sbInfo.nMax - sbInfo.nPage;
	int inc = HIWORD(GetDialogBaseUnits());

	switch (nSBCode)
	{
	case SB_LINEUP:
		if (sbInfo.nPos > sbInfo.nMin)
		{
			sbInfo.nPos = max((sbInfo.nPos - inc), sbInfo.nMin);
			sbInfo.fMask = SIF_POS;
			SetScrollInfo(SB_VERT, &sbInfo, 1);
			m_wndProperty->MoveWindow(0, (rect.top + inc) > 0 ? 0 : rect.top + inc, rect.Width(), rect.Height(), 1);
		}
		break;
	case SB_LINEDOWN:
		if (sbInfo.nPos < length)
		{
			sbInfo.nPos = min(sbInfo.nPos + inc, length);
			sbInfo.fMask = SIF_POS;
			SetScrollInfo(SB_VERT, &sbInfo, 1);
			m_wndProperty->MoveWindow(0, (rect.top - inc) < -length ? -length : rect.top - inc, rect.Width(), rect.Height(), 1);
		}
		break;
	case SB_PAGEUP:
		if (sbInfo.nPos != sbInfo.nMin)
		{
			m_wndProperty->MoveWindow(0,0, rect.Width(),rect.Height(),1);
			SetScrollPos(SB_VERT, sbInfo.nMin, 1);
		}
		break;
	case SB_PAGEDOWN:
		if (sbInfo.nPos != sbInfo.nMax)
		{
			m_wndProperty->MoveWindow(0, -length, rect.Width(), rect.Height(), 1);
			SetScrollPos(SB_VERT, sbInfo.nMax, 1);
		}
		break;
	case SB_THUMBPOSITION:
		SetScrollPos(SB_VERT, nPos);
		break;
	case SB_THUMBTRACK:
		m_wndProperty->MoveWindow(0, -(int)nPos, rect.Width(), rect.Height(), 1);
		break;
	}
	CFrameWnd::OnVScroll(nSBCode, nPos, pScrollBar); // the Default implementation does nothing, let's keep it for now!
}

void CCompassMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// Check if the current screen is the Property window, and its handle is wellformed.
	if (m_enmCurrentWnd == WND_PROPERTY && m_wndProperty->GetSafeHwnd())
	{
		SCROLLINFO sbInfo;
		CRect orgRect;
		CRect clientRect;

		GetScrollInfo(SB_VERT, &sbInfo);
		
		GetClientRect(&clientRect);
		
		m_wndProperty->GetWindowRect(&orgRect);
		ScreenToClient(&orgRect);

		int newLength = orgRect.Height() - clientRect.Height();
		int oldLength = sbInfo.nMax - sbInfo.nPage;

		if (newLength < 0) //if the propertyPage height is smaller than the height of the parent window
		{
			ShowScrollBar(SB_VERT, FALSE); // we don't need a scrollbar, everything is visible.
		}
		else
		{
			if (sbInfo.nPos == oldLength)
			{
				m_wndProperty->MoveWindow(0, -newLength, clientRect.Width(), orgRect.Height(), 1);
				sbInfo.nPos = newLength;
				goto UPDATE_POS;
			}
		}
		m_wndProperty->MoveWindow(0, 0, orgRect.Width(), orgRect.Height(), 1);
		sbInfo.nPos = sbInfo.nMin;

	UPDATE_POS:
		sbInfo.cbSize = sizeof(SCROLLINFO);
		sbInfo.fMask = SIF_POS|SIF_PAGE;
		sbInfo.nPage = clientRect.Height();
		SetScrollInfo(SB_VERT, &sbInfo, 1);
	}
}
