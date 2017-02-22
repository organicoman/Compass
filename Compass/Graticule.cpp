// Graticule.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "Graticule.h"


// CGraticule

IMPLEMENT_DYNAMIC(CGraticule, CStatic)

CGraticule::CGraticule()
{
}

CGraticule::~CGraticule()
{
}


BEGIN_MESSAGE_MAP(CGraticule, CStatic)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CGraticule message handlers




BOOL CGraticule::Create( CWnd* pParentWnd, const RECT& rect)
{
	// TODO: Add your specialized code here and/or call the base class

	LPCTSTR lpszText = _T("");
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_EX_TRANSPARENT;
	m_rcSize = rect;
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd);
}

void CGraticule::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	dc.SelectStockObject(HOLLOW_BRUSH);
	dc.SelectStockObject(WHITE_PEN);

	dc.Ellipse(m_rcSize);
	m_rcSize.DeflateRect(CSize(10, 10));
	dc.Ellipse(m_rcSize);
}


void CGraticule::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct/*lpDrawItemStruct*/)
{

	// TODO:  Add your code to draw the specified item
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	//dc.SelectStockObject(HOLLOW_BRUSH);
	//dc.SelectStockObject(BLACK_PEN);

	dc.Ellipse(m_rcSize);
	m_rcSize.DeflateRect(CSize(10, 10));
	dc.Ellipse(m_rcSize);
}

