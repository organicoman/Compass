// PropertyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "PropertyWnd.h"


// CPropertyWnd

IMPLEMENT_DYNAMIC(CPropertyWnd, CPropertySheet)

CPropertyWnd::CPropertyWnd()
{
}

CPropertyWnd::CPropertyWnd(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&pageQuant);
	AddPage(&pageElemt);
	AddPage(&pageAcqui_Displ);
}

CPropertyWnd::~CPropertyWnd()
{
}


BEGIN_MESSAGE_MAP(CPropertyWnd, CPropertySheet)
END_MESSAGE_MAP()


// CPropertyWnd message handlers
void CPropertyWnd::AdjustPagesLayout()
{
	EnableDynamicLayout(1);
	CMFCDynamicLayout* layout = GetDynamicLayout();
	CTabCtrl* TabCtrl = GetTabControl();
	// first adjust the size of the TabCtrl's display area, 
	// in case the main window was maximized before switching to PrptyWnd
	CRect rect;
	TabCtrl->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	CRect temp;
	GetClientRect(&temp);
	int width = temp.Width();
	
	rect.right = width;
	
	TabCtrl->MoveWindow(&rect, TRUE);

	// then add the TabCtrl and the defacto propertypage (quantification) to the DynamicLayout Manager of the PrptyWnd
	layout->Create(this);
	CMFCDynamicLayout::MoveSettings move;
	CMFCDynamicLayout::SizeSettings size;
	size.m_nXRatio = 100;
	layout->AddItem(TabCtrl->GetSafeHwnd(), move, size);
	
	layout->AddItem(pageQuant.GetSafeHwnd(), move, size);
}