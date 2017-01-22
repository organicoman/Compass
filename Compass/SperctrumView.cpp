// SperctrumView.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "SperctrumView.h"


// CSperctrumView

IMPLEMENT_DYNCREATE(CSperctrumView, CView)

CSperctrumView::CSperctrumView()
{

}

CSperctrumView::~CSperctrumView()
{
}

BEGIN_MESSAGE_MAP(CSperctrumView, CView)
END_MESSAGE_MAP()


// CSperctrumView drawing

void CSperctrumView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CSperctrumView diagnostics

#ifdef _DEBUG
void CSperctrumView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSperctrumView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSperctrumView message handlers
