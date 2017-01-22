// CamView.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "CamView.h"


// CCamView

IMPLEMENT_DYNCREATE(CCamView, CView)

CCamView::CCamView()
{

}

CCamView::~CCamView()
{
}

BEGIN_MESSAGE_MAP(CCamView, CView)
END_MESSAGE_MAP()


// CCamView drawing

void CCamView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CCamView diagnostics

#ifdef _DEBUG
void CCamView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCamView message handlers
