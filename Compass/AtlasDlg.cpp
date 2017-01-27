
// AtlasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "AtlasDlg.h"
#include "afxdialogex.h"

// Non Framework includes
//
#include "AtlasMainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAtlasDlg dialog

CAtlasDlg::CAtlasDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ATLAS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//prepare the Table to receive data
	m_table.Table.SetSize(12, 4);
}

void CAtlasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	// Get the underlaying CListCtrl into m_ListCtrl
	DDX_Control(pDX, IDC_LIST_TABLE, m_ListCtrl);
	// Create a temporary container
	//sData tmpData;
	//DDX_Text(pDX, IDC_COMBO_POS, tmpData.pos);
	//	DDV_MinMaxUInt(pDX, tmpData.pos, 1, 12);
	//DDX_Text(pDX, IDC_EDIT_KV, tmpData.kV);
	//DDX_Text(pDX, IDC_EDIT_MA, tmpData.mA);
	//DDX_CBStringExact(pDX, IDC_COMBO_FILTER, tmpData.filter);
	//DDX_CBStringExact(pDX, IDC_COMBO_ATMOSPHERE, tmpData.atmoshpere);
	//DDX_Text(pDX, IDC_BROWSE_FOLDER, tmpData.path);
	//DDX_Text(pDX, IDC_COMBO_PRESET, tmpData.preset);
	//DDX_Check(pDX, IDC_CHECK_SAVEIMG, tmpData.capture);
	////call this to populate the table
	//PopulateTable(tmpData);
}

BEGIN_MESSAGE_MAP(CAtlasDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_BUTTON_CAMERA, &CAtlasDlg::OnIdwCamera)
	ON_COMMAND(IDC_BUTTON_CAROUSEL, &CAtlasDlg::OnIdwCarousel)
	ON_COMMAND(IDC_BUTTON_SPECTRUM, &CAtlasDlg::OnIdwSpectrum)
	ON_COMMAND(IDC_BUTTON_PROPERTY, &CAtlasDlg::OnIdwProperty)
	ON_COMMAND(IDC_BUTTON_ADD, &CAtlasDlg::OnButtonAdd)
END_MESSAGE_MAP()


// CAtlasDlg message handlers

BOOL CAtlasDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetBackgroundColor(RGB(54, 69, 79));
	// Initialize the Data Table (m_ListCtrl)
	SetupTable();

	// Create the MainFrame on the Heap.
	CAtlasMainFrame* Frame = new CAtlasMainFrame;
	ASSERT_VALID(Frame);
	if (Frame)
	{
		m_pCWndAtlas = Frame;
		if (!CreateAtlasFrame())   //Helper Function to create the Atlas window.
			return FALSE;
		CMFCDynamicLayout* Layout = GetDynamicLayout(); //Get the Layout manager of the Dialog and insert the Atlas Wnd in it
		CMFCDynamicLayout::MoveSettings move;
		CMFCDynamicLayout::SizeSettings size;
		size.m_nXRatio = 100;
		size.m_nYRatio = 100;
		Layout->AddItem(m_pCWndAtlas->GetSafeHwnd(), move, size);
	}

	// Set All the Middle Toolbar Controls to their Default Values
	SendDlgItemMessage(IDC_COMBO_POS, CB_SETCURSEL);// index n=0 is implied, check the doc.
	SendDlgItemMessage(IDC_COMBO_PRESET, CB_SETCURSEL);
	SendDlgItemMessage(IDC_COMBO_ATMOSPHERE, CB_SETCURSEL);
	SendDlgItemMessage(IDC_COMBO_FILTER, CB_SETCURSEL);
	
	ShowWindow(SW_NORMAL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAtlasDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAtlasDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CAtlasDlg Helper Functions

// This function is called from the method: OnInitDialog().
void CAtlasDlg::SetupTable()
{
	//Populate the Table's Column Headers.
	m_ListCtrl.InsertColumn(0, _T("Sel"), LVCFMT_LEFT, 36, 0);
	m_ListCtrl.InsertColumn(1, _T("Pos"), LVCFMT_LEFT, 30, 1);
	m_ListCtrl.InsertColumn(2, _T("Name"), LVCFMT_LEFT, 100, 2);
	//m_ListCtrl.InsertColumn(3, _T("kV"), LVCFMT_LEFT, 42, 3);
	//m_ListCtrl.InsertColumn(4, _T("mA"), LVCFMT_LEFT, 42, 4);
	//m_ListCtrl.InsertColumn(5, _T("c/s"), LVCFMT_LEFT, 42, 5);
	//m_ListCtrl.InsertColumn(6, _T("Filter"), LVCFMT_LEFT, 48, 6);
	m_ListCtrl.InsertColumn(3, _T("Template"), LVCFMT_LEFT, 66, 3);
	m_ListCtrl.InsertColumn(4, _T("Folder"), LVCFMT_LEFT, 120, 4);
	//m_ListCtrl.InsertColumn(9, _T("Preset"), LVCFMT_LEFT, 48, 9);
	//m_ListCtrl.InsertColumn(10, _T("Keep Capture"), LVCFMT_LEFT, 80, 10);
}

// Helper function to Create the Main FrameWnd
BOOL CAtlasDlg::CreateAtlasFrame()
{
	CRect rect(3, 66, 508, 362);
	MapDialogRect(&rect);
	
	return m_pCWndAtlas->Create(NULL, NULL, 0UL, rect, this);
}

void CAtlasDlg::OnIdwCamera()
{
	// TODO: Add your command handler code here
	m_pCWndAtlas->SwitchWnd(CAtlasMainFrame::WND_CAMERA);
}

void CAtlasDlg::OnIdwCarousel()
{
	// TODO: Add your command handler code here
	m_pCWndAtlas->SwitchWnd(CAtlasMainFrame::WND_CAROUSEL);
}

void CAtlasDlg::OnIdwSpectrum()
{
	// TODO: Add your command handler code here
	m_pCWndAtlas->SwitchWnd(CAtlasMainFrame::WND_SPECTRUM);
}

void CAtlasDlg::OnIdwProperty()
{
	// TODO: Add your command handler code here
	m_pCWndAtlas->SwitchWnd(CAtlasMainFrame::WND_PROPERTY);
}

void CAtlasDlg::OnButtonAdd()
{
	// TODO: Add your command handler code here
	//This Message can come from the "ADD" toolbar button of the CarouselWnd or CameraWnd
    //check the type of the actual screen
	using MNFRM_CW = CAtlasMainFrame::CURRENTWND; // just using an Alias to that scope
	
	switch (m_pCWndAtlas->GetCurrentScreen())
	{
	case MNFRM_CW::WND_CAROUSEL:
		// call this function to get the data from the controls  then populate the table
		UpdateData(TRUE); //CWnd method TRUE means perform data exchange with the controls
		break;
	case MNFRM_CW::WND_CAMERA:
		// Id on't know yet what his button do??
		break;
	}
}


// Populate the table using the controls in the Header toolbar
void CAtlasDlg::PopulateTable(sData& tmpData)
{
	CWaitCursor wcur;
	static unsigned char counter = 1;
	tmpData.name += counter;
	tmpData.spot += counter;
	tmpData.cs = 12.34;
	counter++;
	UINT idx = m_table.Table.Add(tmpData);
	//update the listCtrl with these data

}

