// CarouselView.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "CarouselView.h"


// CCarouselView

IMPLEMENT_DYNCREATE(CCarouselView, CView)

CCarouselView::CCarouselView()
{

}

CCarouselView::~CCarouselView()
{
}

BEGIN_MESSAGE_MAP(CCarouselView, CView)
END_MESSAGE_MAP()


// CCarouselView drawing

void CCarouselView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CCarouselView diagnostics

#ifdef _DEBUG
void CCarouselView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCarouselView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCarouselView message handlers
