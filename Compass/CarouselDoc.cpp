// CarouselDoc.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "CarouselDoc.h"


// CCarouselDoc

IMPLEMENT_DYNCREATE(CCarouselDoc, CDocument)

CCarouselDoc::CCarouselDoc()
{
}

BOOL CCarouselDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCarouselDoc::~CCarouselDoc()
{
}


BEGIN_MESSAGE_MAP(CCarouselDoc, CDocument)
END_MESSAGE_MAP()


// CCarouselDoc diagnostics

#ifdef _DEBUG
void CCarouselDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CCarouselDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CCarouselDoc serialization

void CCarouselDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CCarouselDoc commands
