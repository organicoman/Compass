// RoundButton.cpp : implementation file
//

#include "stdafx.h"
#include "RoundButton.h"

// CRoundButton

IMPLEMENT_DYNAMIC(CRoundButton, CMFCButton)

CRoundButton::CRoundButton():
	m_State(ST_DISABLED)
{
}

CRoundButton::~CRoundButton()
{
}

BEGIN_MESSAGE_MAP(CRoundButton, CMFCButton)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

// CRoundButton message handlers


void CRoundButton::OnDraw(CDC* pDC, const CRect& rect, UINT uiState)
{
	// TODO: Add your specialized code here and/or call the base class
	COLORREF cDisabled = RGB(80, 80, 80);
	COLORREF cDisabledPen = RGB(10, 10, 10);

	COLORREF cActive = RGB(0, 160, 0);
	COLORREF cActivePen = RGB(0, 90, 50);

	COLORREF cHotLit = RGB(0, 140, 200);
	COLORREF cHotLitPen = RGB(0, 80, 200);
	
	COLORREF cSelected = RGB(0, 190, 0);
	COLORREF cSelectedPen= RGB(230, 0, 0);

	CPen ActivePen{ PS_SOLID, 1, cActivePen };
	CPen SelectPen{ PS_SOLID, 3, cSelectedPen };
	CPen HotLitPen{ PS_SOLID, 2, cHotLitPen };
	CPen DisablePen{ PS_SOLID, 1, cDisabledPen };

	CBrush DisabledBrush{ cDisabled };
	CBrush ActiveBrush{ cActive };
	CBrush HotLitBrush{ cHotLit };
	CBrush SelectedBrush{ cSelected };

	
	m_CircleRgn.CreateEllipticRgnIndirect(rect);

	////
	CBrush* oldBrush = nullptr;
	CPen* oldPen = nullptr;
	////
	// Set the Clipping Region to the Circle AND the OldRgn

	pDC->SelectClipRgn(&m_CircleRgn, RGN_AND);
	switch (uiState)
	{
	case ODS_DISABLED:
		oldPen = pDC->SelectObject(&DisablePen);
		oldBrush = pDC->SelectObject(&DisabledBrush);
		break;
	case ODS_HOTLIGHT:
		oldPen = pDC->SelectObject(&HotLitPen);
		oldBrush = pDC->SelectObject(&HotLitBrush);
		break;
	case ODS_SELECTED:
		oldPen = pDC->SelectObject(&SelectPen);
		oldBrush = pDC->SelectObject(&SelectedBrush);
		break;
	default: //Active
		oldPen = pDC->SelectObject(&ActivePen);
		oldBrush = pDC->SelectObject(&ActiveBrush);
	}

	pDC->Ellipse(rect);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	return CMFCButton::OnDraw(pDC, rect, uiState);
}


BOOL CRoundButton::Create(LPCTSTR lpszCaption, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | BS_PUSHBUTTON | BS_OWNERDRAW;
	return CMFCButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
}


void CRoundButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DRAWITEMSTRUCT drawItemStruct;
	drawItemStruct.CtlType = ODT_BUTTON;
	drawItemStruct.CtlID = GetDlgCtrlID();
	drawItemStruct.itemID = ODA_DRAWENTIRE;

	if (m_State == ST_DISABLED)
	{
		drawItemStruct.itemState = 0;
		m_State = ST_ENABLED;
	}
	else
	{
		drawItemStruct.itemState = ODS_DISABLED;
		m_State = ST_DISABLED;
	}
	
	drawItemStruct.hwndItem = GetSafeHwnd();
	drawItemStruct.hDC = GetDC()->m_hDC;
	GetClientRect(&drawItemStruct.rcItem);
	
	if (m_CircleRgn.PtInRegion(point))
	{
		SendMessage(WM_DRAWITEM, GetDlgCtrlID(), (LPARAM)&drawItemStruct);
	}

	CMFCButton::OnRButtonDown(nFlags, point);
}


void CRoundButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DRAWITEMSTRUCT drawItemStruct;
	drawItemStruct.CtlType = ODT_BUTTON;
	drawItemStruct.CtlID = GetDlgCtrlID();
	
	if (m_State == ST_ENABLED)
	{
		drawItemStruct.itemState = ODS_SELECTED | ODS_HOTLIGHT;
		drawItemStruct.itemID = ODA_DRAWENTIRE | ODA_SELECT;
	}
	else
	{
		drawItemStruct.itemState = ODS_DISABLED;
		drawItemStruct.itemID = ODA_DRAWENTIRE;
	}

	drawItemStruct.hwndItem = GetSafeHwnd();
	drawItemStruct.hDC = GetDC()->m_hDC;
	GetClientRect(&drawItemStruct.rcItem);

	if (m_CircleRgn.PtInRegion(point))
	{
		SendMessage(WM_DRAWITEM, GetDlgCtrlID(), (LPARAM)&drawItemStruct);
	}

	CMFCButton::OnLButtonDown(nFlags, point);
}


void CRoundButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DRAWITEMSTRUCT drawItemStruct;
	drawItemStruct.CtlType = ODT_BUTTON;
	drawItemStruct.CtlID = GetDlgCtrlID();

	if (m_State == ST_ENABLED)
	{
		drawItemStruct.itemState = ODS_HOTLIGHT;
		drawItemStruct.itemID = ODA_DRAWENTIRE | ODA_SELECT;
	}
	else
	{
		drawItemStruct.itemState = ODS_DISABLED;
		drawItemStruct.itemID = ODA_DRAWENTIRE;
	}

	drawItemStruct.hwndItem = GetSafeHwnd();
	drawItemStruct.hDC = GetDC()->m_hDC;
	GetClientRect(&drawItemStruct.rcItem);

	if (m_CircleRgn.PtInRegion(point))
	{
		SendMessage(WM_DRAWITEM, GetDlgCtrlID(), (LPARAM)&drawItemStruct);
	}

	CMFCButton::OnMouseHover(nFlags, point);
}
