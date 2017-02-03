// ElementPage.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "ElementPage.h"
#include "afxdialogex.h"


// CElementPage dialog

IMPLEMENT_DYNAMIC(CElementPage, CMFCPropertyPage)

CElementPage::CElementPage()
	: CMFCPropertyPage(IDD_PROPPAGE_ELMNTS)
{

}

CElementPage::~CElementPage()
{
}

void CElementPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CElementPage, CMFCPropertyPage)
END_MESSAGE_MAP()


// CElementPage message handlers
