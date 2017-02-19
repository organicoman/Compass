// CompassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "CompassDlg.h"
#include "afxdialogex.h"

// Non Framework includes
//
#include "CompassMainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCompassDlg dialog

CCompassDlg::CCompassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMPASS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCompassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	// Get the underlaying CListCtrl into m_ListCtrl
	DDX_Control(pDX, IDC_LIST_TABLE, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CCompassDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_BUTTON_CAMERA, &CCompassDlg::OnIdwCamera)
	ON_COMMAND(IDC_BUTTON_CAROUSEL, &CCompassDlg::OnIdwCarousel)
	ON_COMMAND(IDC_BUTTON_SPECTRUM, &CCompassDlg::OnIdwSpectrum)
	ON_COMMAND(IDC_BUTTON_PROPERTY, &CCompassDlg::OnIdwProperty)
	ON_COMMAND(IDC_BUTTON_ADD, &CCompassDlg::OnButtonAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TABLE, &CCompassDlg::OnDblclkListTable)
END_MESSAGE_MAP()


// CCompassDlg message handlers

BOOL CCompassDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetBackgroundColor(RGB(54, 69, 79));
	// Initialize the Data Table (m_ListCtrl)
	SetupTable();
	
	// Give each Button the Appropriate Icon.
	SetupIcons();
	// Create the MainFrame on the Heap.
	CCompassMainFrame* Frame = new CCompassMainFrame;
	ASSERT_VALID(Frame);
	if (Frame)
	{
		m_pCWndCompass = Frame;
		if (!CreateCompassFrame())   //Helper Function to create the Atlas window.
			return FALSE;
		CMFCDynamicLayout* Layout = GetDynamicLayout(); //Get the Layout manager of the Dialog and insert the Atlas Wnd in it
		CMFCDynamicLayout::MoveSettings move;
		CMFCDynamicLayout::SizeSettings size;
		size.m_nXRatio = 100;
		size.m_nYRatio = 100;
		Layout->AddItem(m_pCWndCompass->GetSafeHwnd(), move, size);
	}

	// Set All the Middle Tool-bar Controls to their Default Values
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

void CCompassDlg::OnPaint()
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
HCURSOR CCompassDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CCompassDlg Helper Functions

// This function is called from the method: OnInitDialog().
void CCompassDlg::SetupTable()
{
	//Populate the Table's Column Headers.
	m_ListCtrl.InsertColumn(0, _T("Slot"), LVCFMT_LEFT, 42, 0);
	m_ListCtrl.InsertColumn(1, _T("Spot"), LVCFMT_CENTER, 42, 1);
	m_ListCtrl.InsertColumn(2, _T("Name"), LVCFMT_CENTER, 100, 2);
	m_ListCtrl.InsertColumn(3, _T("Template"), LVCFMT_CENTER, 100, 3);
	m_ListCtrl.InsertColumn(4, _T("Folder"), LVCFMT_CENTER, 120, 4);
	DWORD style = m_ListCtrl.GetExtendedStyle();
	style |= LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES;
	m_ListCtrl.SetExtendedStyle(style);
}

void CCompassDlg::SetupIcons()
{
	CMFCToolBarImages images;
	BOOL bResult;
	images.SetImageSize(CSize(48, 48));
	bResult = images.Load(IDB_PNG_GLOBALS);

	SendDlgItemMessage(IDC_BUTTON_PROPERTY, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(0));
	SendDlgItemMessage(IDC_BUTTON_REPORT, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(1));
	SendDlgItemMessage(IDC_BUTTON_SPECTRUM, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(2));
	SendDlgItemMessage(IDC_BUTTON_CAMERA, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(3));
	SendDlgItemMessage(IDC_BUTTON_CAROUSEL, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(4));

	images.SetImageSize(CSize(24, 24));
	bResult = images.Load(IDB_PNG_TABLE);

	SendDlgItemMessage(IDC_BUTTON_START, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(0));
	SendDlgItemMessage(IDC_BUTTON_LOAD, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(1));
	SendDlgItemMessage(IDC_BUTTON_SAVE, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(2));
	SendDlgItemMessage(IDC_BUTTON_DELETE, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(3));
	SendDlgItemMessage(IDC_BUTTON_MOVE, BM_SETIMAGE, IMAGE_ICON, (LPARAM)images.ExtractIcon(4));
	
}

// Helper function to Create the Main FrameWnd
BOOL CCompassDlg::CreateCompassFrame()
{
	CRect rect(3, 66, 508, 362);
	MapDialogRect(&rect);
	
	return m_pCWndCompass->Create(NULL, NULL, 0UL, rect, this);
}

void CCompassDlg::OnIdwCamera()
{
	// TODO: Add your command handler code here
	m_pCWndCompass->SwitchWnd(CCompassMainFrame::WND_CAMERA);
}

void CCompassDlg::OnIdwCarousel()
{
	// TODO: Add your command handler code here
	m_pCWndCompass->SwitchWnd(CCompassMainFrame::WND_CAROUSEL);
}

void CCompassDlg::OnIdwSpectrum()
{
	// TODO: Add your command handler code here
	m_pCWndCompass->SwitchWnd(CCompassMainFrame::WND_SPECTRUM);
}

void CCompassDlg::OnIdwProperty()
{
	// TODO: Add your command handler code here
	m_pCWndCompass->SwitchWnd(CCompassMainFrame::WND_PROPERTY);
}


void CCompassDlg::OnButtonAdd()
{
	// TODO: Add your command handler code here
	// this Message is directly forwarded from the child of the CComapssMainFrame object.

	// let's collect the selected Slots
	ASSERT(m_pCWndCompass);
	if (m_pCWndCompass)
	{
		std::vector<UINT> selection = m_pCWndCompass->GetSelectedSlots();
		// Add them to the Table
		PopulateTable(selection);
	}
}

void CCompassDlg::PopulateTable(std::vector<UINT>& selection)
{
	LVFINDINFO info{ 0 };

	for (size_t i = 0; i < selection.size(); i++)
	{
		CString s;
		UINT SlotIdx = selection[i] + 1; // add one because it is Zero based.
		s.Format(_T("%d"), SlotIdx);
		info.psz = s; // search for text equal to "s"
		info.flags = LVFI_STRING;
		int idx = m_ListCtrl.FindItem(&info);
		int sub;
		if (idx == -1)
		{
			int BottomPos = m_ListCtrl.GetItemCount();
			sub = m_ListCtrl.InsertItem(BottomPos, s);
			m_ListCtrl.SetItemText(sub, 1, _T("1")); // the first addition is always the first Spot on the Slot.
			m_ListCtrl.SetItemData(sub, SlotIdx); //save the slot index as an LPARAM data for comparison later
			CString name;
			name.Format(_T("Slot-%d / Spot-%d"), SlotIdx, 1); // Just a default Name
			m_ListCtrl.SetItemText(sub, 2, name);
			continue;
		}	
		int count = 2;
		info.flags = LVFI_PARAM; // search for the item's LPARAM data
		info.lParam = SlotIdx;
		while ((sub = m_ListCtrl.FindItem(&info, idx)) != -1)
		{
			count++; // count how many spots for this Slot.
			idx = sub;
		}
		s.Format(_T("%d"), count);
		m_ListCtrl.InsertItem(idx + 1, _T(""));
		m_ListCtrl.SetItemText(idx + 1, 1, s);
		m_ListCtrl.SetItemData(idx + 1, SlotIdx);
		CString name;
		name.Format(_T("Slot-%d / Spot-%d"), SlotIdx, count); // Just a Default Name
		m_ListCtrl.SetItemText(idx + 1, 2, name);
	}

}

void CCompassDlg::OnDblclkListTable(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// it the User DBLCLicks the "template" cell (- position ==3), then open the Template Dialog.
	if (pNMItemActivate->iSubItem == 3)
	{
		// TODO: Implement the Templates Dialogs.
	}
}
