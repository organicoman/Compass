// CamDoc.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "CamDoc.h"


// CCamDoc

IMPLEMENT_DYNCREATE(CCamDoc, CDocument)

CCamDoc::CCamDoc()
{
}

BOOL CCamDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCamDoc::~CCamDoc()
{
}


BEGIN_MESSAGE_MAP(CCamDoc, CDocument)
END_MESSAGE_MAP()


// CCamDoc diagnostics

#ifdef _DEBUG
void CCamDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CCamDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CCamDoc serialization

void CCamDoc::Serialize(CArchive& ar)
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


// CCamDoc commands
