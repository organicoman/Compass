// CarouselWnd.cpp : implementation file
//
#pragma once

#include "stdafx.h"
#include "Compass.h"
#include "CarouselWnd.h"

const double pi = acos(-1.0);
// CCarouselWnd

IMPLEMENT_DYNCREATE(CCarouselWnd, CFrameWndEx)

CCarouselWnd::CCarouselWnd(UINT nSlots):
	m_nSlots(nSlots)
{
	sSlot slot;
	slot.enmState = sSlot::SLOT_DISABLED;
	slot.bChanged = TRUE;
	
	// All slots start in Disabled state, and need first Visual update
	for (UINT i=0; i<nSlots;i++)
	{
		m_SlotCollection.emplace_back(slot);
	}
	m_SlotCollection.shrink_to_fit();
}

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
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDC_BUTTON_ADD, &CCarouselWnd::OnButtonAdd)
END_MESSAGE_MAP()

// CCarouselWnd message handlers

int CCarouselWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	// Initialize the Add and Cam Buttons 32x32 size
	int w = 32;
	int h = 32;
	int xPos = 3;
	int yPos = 3;
	
	m_CamBtn.Create(_T("CAM"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(xPos, yPos, xPos + w, yPos + h), this, IDC_BUTTON_VIEW);
	m_AddBtn.Create(_T("ADD"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(xPos + w + 3, yPos, xPos + 2 * w + 3, yPos + h), this, IDC_BUTTON_ADD);

	
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
	CWaitCursor cur;

	if (fCamOnOff == FALSE)
	{
		CRect rect;
		_SetRect(rect);

		m_Camera = new CCameraWnd;
		ASSERT(m_Camera);
		
		if (!m_Camera->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rect, this, 1234/*arbitrary*/)) 
		{
			TRACE0("Failure: error while creating the camera view window\n");
		}
		fCamOnOff = TRUE;
	}
	else
	{
		_CleanUp();
	}
	
}

void CCarouselWnd::_CleanUp()
{
	if (fCamOnOff)
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
					   // Initialize the Carousel and the Slots bounding rectangles.
	
	CRect CarouselBoundBox;

	CRect screen;
	GetClientRect(&screen);
	int x = screen.Width() / 2;
	int y = screen.Height() / 2;
	int radius = x < y ? (x - 20) : (y - 20);
	
	// Draw the Carousel
	CarouselBoundBox = CRect(x - radius, y - radius, x + radius, y + radius);
	dc.Ellipse(CarouselBoundBox);

	// the Slots now.	
	double Angle = 2 * pi / m_nSlots;
	double halfAngle = Angle/2; 
	// Slot radius
	double rad = radius * (1 - (1 / (sin(halfAngle) + 1))) - 3 /*spacing*/;
	// Draw the Slots
	for (UINT n = 0; n < m_nSlots; n++)
	{
		double SlotCenterX = (radius - rad - 5/*spacing*/)* cos(n*Angle);
		double SlotCenterY = (radius - rad - 5/*spacing*/)* sin(n*Angle);
		CRect rect;
		rect.top = SlotCenterY - rad;
		rect.left = SlotCenterX - rad;
		rect.bottom = SlotCenterY + rad;
		rect.right = SlotCenterX + rad;
		rect.OffsetRect(x, y);
		_DrawSlot(n, &dc, &rect);
		// save it in the Slot collection structure.
		
		m_SlotCollection[n].rcHitRgn = rect;
	}
}

void CCarouselWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	if (m_Camera != nullptr)
	{
		CRect rect;

		_SetRect(rect);

		m_Camera->MoveWindow(&rect);
	}
}

void CCarouselWnd::_SetRect(CRect& rect)
{
	GetClientRect(&rect);
	int x = rect.Width() / 2;
	int y = 5;
	rect.left = x - 5;
	rect.top = y;
	rect.right = rect.right - 5;
	rect.bottom = rect.Height() / 2 - 5;
}

void CCarouselWnd::_DrawSlot(UINT pos, CPaintDC * pDC, CRect * boundingBox) const
{
	sSlot::SLOT_STATE ActualState = m_SlotCollection[pos].enmState;
	BOOL needUpdate = m_SlotCollection[pos].bChanged;
	CPen* oldPen= nullptr;
	CBrush* oldBrush = nullptr;

	if (needUpdate == TRUE)
	{
		switch (ActualState)
		{
		case sSlot::SLOT_DISABLED:
			oldPen = pDC->SelectObject(const_cast<CPen*>(&DisabledPen));
			oldBrush = pDC->SelectObject(const_cast<CBrush*>(&DisabledBrush));
			break;
		case sSlot::SLOT_ENABLED:
			oldPen = pDC->SelectObject(const_cast<CPen*>(&EnabledPen));
			oldBrush = pDC->SelectObject(const_cast<CBrush*>(&EnabledBrush));
			break;
		case sSlot::SLOT_SELECTED:
			oldPen = pDC->SelectObject(const_cast<CPen*>(&SelectedPen));
			oldBrush = pDC->SelectObject(const_cast<CBrush*>(&SelectedBrush));
			break;
		case sSlot::SLOT_ADDED:
			oldPen = pDC->SelectObject(const_cast<CPen*>(&HotLitPen));
			oldBrush = pDC->SelectObject(const_cast<CBrush*>(&HotLitBrush));
			break;
		}
		pDC->Ellipse(boundingBox);
		
		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}

}


void CCarouselWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	for (UINT n = 0; n < m_nSlots; n++) // for obscure reason the following form of the for loop doesn't work "for(auto slot:m_SlotCollection)"
	{
		CRgn hitRgn;
		CRect rect = m_SlotCollection[n].rcHitRgn;
		rect.InflateRect(3, 3); // just for smooth drawing
		hitRgn.CreateEllipticRgnIndirect(&rect);
		if (hitRgn.PtInRegion(point))
		{
			sSlot::SLOT_STATE state = m_SlotCollection[n].enmState;
			if (state != sSlot::SLOT_SELECTED && state != sSlot::SLOT_ADDED)
			{
				switch (state)
				{
				case sSlot::SLOT_DISABLED:
					m_SlotCollection[n].enmState = sSlot::SLOT_ENABLED;
					m_SlotCollection[n].bChanged = TRUE;
					break;
				case sSlot::SLOT_ENABLED:
					m_SlotCollection[n].enmState = sSlot::SLOT_DISABLED;
					m_SlotCollection[n].bChanged = TRUE;
					break;
				}
				InvalidateRgn(&hitRgn);
				UpdateWindow();
			}
		}
	}

	CFrameWndEx::OnRButtonDown(nFlags, point);
}


void CCarouselWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	for (UINT n = 0; n < m_nSlots; n++) 
	{
		CRgn hitRgn;
		CRect rect = m_SlotCollection[n].rcHitRgn;
		rect.InflateRect(3, 3); // just for smooth drawing
		hitRgn.CreateEllipticRgnIndirect(&rect);
		if (hitRgn.PtInRegion(point))
		{
			sSlot::SLOT_STATE state = m_SlotCollection[n].enmState;
			if (state == sSlot::SLOT_ENABLED) // change state only when Slot is Enabled
			{
				m_SlotCollection[n].enmState = sSlot::SLOT_SELECTED;
				m_SlotCollection[n].bChanged = TRUE;
				InvalidateRgn(&hitRgn);
				UpdateWindow();
			}
			else
			{
				m_SlotCollection[n].bChanged = FALSE;
			}
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}


void CCarouselWnd::OnButtonAdd()
{
	// TODO: Add your command handler code here
	for (UINT n = 0; n < m_nSlots; n++)
	{
		sSlot::SLOT_STATE state = m_SlotCollection[n].enmState;
		if (state == sSlot::SLOT_SELECTED)
		{
			CRgn hitRgn;
			CRect rect = m_SlotCollection[n].rcHitRgn;
			rect.InflateRect(3, 3); // just for smooth drawing
			m_SlotCollection[n].enmState = sSlot::SLOT_ADDED;
			m_SlotCollection[n].bChanged = TRUE;
			InvalidateRgn(&hitRgn);
			UpdateWindow();
		}
	}
}
