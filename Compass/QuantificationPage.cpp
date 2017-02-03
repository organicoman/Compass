// QuantificationPage.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "QuantificationPage.h"
#include "afxdialogex.h"


// CQuantificationPage dialog

IMPLEMENT_DYNAMIC(CQuantificationPage, CMFCPropertyPage)

CQuantificationPage::CQuantificationPage()
	: CMFCPropertyPage(IDD_PROPPAGE_QUANT)
{

}

CQuantificationPage::~CQuantificationPage()
{
}

void CQuantificationPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	// Get the two Tables
	DDX_Control(pDX, IDC_COMPONENT_TABLE, m_listCtrl_Compnt);
	DDX_Control(pDX, IDC_ANALYZE_TABLE, m_listCtrl_Analyze);
}


BEGIN_MESSAGE_MAP(CQuantificationPage, CMFCPropertyPage)
END_MESSAGE_MAP()


// CQuantificationPage message handlers


// Adds the Headers to the Tables
void CQuantificationPage::SetupTables()
{
	// Setting the Componenets Table
	m_listCtrl_Compnt.InsertColumn(0, _T(""), LVCFMT_LEFT, 20, 0);
	m_listCtrl_Compnt.InsertColumn(1, _T("Component"), LVCFMT_LEFT, 80, 1);
	m_listCtrl_Compnt.InsertColumn(2, _T("type"), LVCFMT_LEFT, 40, 2);
	m_listCtrl_Compnt.InsertColumn(3, _T("Mole Conc."), LVCFMT_LEFT, 70, 3);
	m_listCtrl_Compnt.InsertColumn(4, _T("Conc."), LVCFMT_LEFT, 40, 4);
	m_listCtrl_Compnt.InsertColumn(5, _T("Units"), LVCFMT_LEFT, 50, 5);

	// Setting the Analyze Table
	m_listCtrl_Analyze.InsertColumn(0, _T("Elmnts"), LVCFMT_LEFT, 50, 0);
	m_listCtrl_Analyze.InsertColumn(1, _T("Line"), LVCFMT_LEFT, 40, 1);
	m_listCtrl_Analyze.InsertColumn(2, _T("Decon"), LVCFMT_LEFT, 44, 2);
	m_listCtrl_Analyze.InsertColumn(3, _T("Low keV"), LVCFMT_LEFT, 70, 3);
	m_listCtrl_Analyze.InsertColumn(4, _T("High keV"), LVCFMT_LEFT, 70, 4);
	m_listCtrl_Analyze.InsertColumn(5, _T("Quant"), LVCFMT_LEFT, 60, 5);
	m_listCtrl_Analyze.InsertColumn(6, _T("Calib File"), LVCFMT_LEFT, 84, 6);
}


BOOL CQuantificationPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	SetupTables();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
