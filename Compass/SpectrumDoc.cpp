// SpectrumDoc.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "SpectrumDoc.h"


// CSpectrumDoc

IMPLEMENT_DYNCREATE(CSpectrumDoc, CDocument)

CSpectrumDoc::CSpectrumDoc()
{
}

BOOL CSpectrumDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSpectrumDoc::~CSpectrumDoc()
{
}


BEGIN_MESSAGE_MAP(CSpectrumDoc, CDocument)
END_MESSAGE_MAP()


// CSpectrumDoc diagnostics

#ifdef _DEBUG
void CSpectrumDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSpectrumDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSpectrumDoc serialization

void CSpectrumDoc::Serialize(CArchive& ar)
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


// CSpectrumDoc commands
