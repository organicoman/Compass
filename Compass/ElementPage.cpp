// ElementPage.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "ElementPage.h"
#include "afxdialogex.h"


// CElementPage dialog

IMPLEMENT_DYNAMIC(CElementPage, CPropertyPage)

CElementPage::CElementPage()
	: CPropertyPage(IDD_PROPPAGE_ELMNTS)
{

}

CElementPage::~CElementPage()
{
}

void CElementPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CElementPage, CPropertyPage)
END_MESSAGE_MAP()


// CElementPage message handlers
