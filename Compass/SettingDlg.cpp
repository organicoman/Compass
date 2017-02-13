// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "SettingDlg.h"


#define COMBO_4_SLOTS 0
#define COMBO_8_SLOTS 1
#define COMBO_12_SLOTS 2
#define COMBO_24_SLOTS 3
#define EMPTY _T("N/A")

// CSettingDlg dialog

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(BOOL bValidRegistry, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTINGDLG, pParent)
{
	// If the Application failed to create a registry profile then create it here and fill it with default values.
	if (bValidRegistry == FALSE)
	{
#define SECTION_CAROUSEL _T("Carousel")
#define N_SLOTS_KEY _T("Number of Slots")
#define SECTION_CAMERA _T("Camera")
#define CAMERA_NAME_KEY _T("Camera Name")
#define CAMERA_ID_KEY _T("Camera Path")


		BOOL ret;

		// Use the Global Variable "theApp"
		ret = theApp.WriteProfileInt(SECTION_CAROUSEL,N_SLOTS_KEY, 8/*by default*/);
		ret = theApp.WriteProfileString(SECTION_CAMERA, CAMERA_NAME_KEY, EMPTY);
		ret = theApp.WriteProfileString(SECTION_CAMERA, CAMERA_ID_KEY, EMPTY);
		
		m_CurrentSetting = { 8, EMPTY, EMPTY };
	}
	else
	{
		m_CurrentSetting = theApp.m_ActualSettings;
	}
	// finally Initialize the "m_CamList" with the Available cameras, using the following function from VideoCapture Utility cpp.
	// in Case this function fails, use the default Camera enumerated by (CameraObj::EnumerateCams) see CCarmeraWnd Class.
	EnumVideoDev(&m_CamList); 
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// Capture the Selection and push it to the "m_CurrentSetting" member variable.

	CString FriendlyName;
	int nSlots;
	if (pDX->m_bSaveAndValidate)
	{
		DDX_CBString(pDX, IDC_COMBO_CAM, FriendlyName);
		m_CurrentSetting.m_strCurrentCamFriendlyName = FriendlyName;
		
		// get the corresponding ID from the "m_CamList" member structure.(already filled)
		int idx = SendDlgItemMessage(IDC_COMBO_CAM, CB_GETCURSEL);
		m_CurrentSetting.m_strCurrentCamPath = m_CamList[idx].strID;

		DDX_Text(pDX, IDC_COMBO_NPOS, nSlots);
		m_CurrentSetting.m_nSlots = nSlots;
		
		return;
	}
	else // reading data from m_CurrentSetting member structure. and adjust the indexes.
	{

	}
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
END_MESSAGE_MAP()

// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	PopulateDlg(m_CamList); // fill the Cam Combo Box with available Cameras FriendlyNames.

	// Choose the number of slots option saved in m_CurrentSetting as the default choice.
	switch (m_CurrentSetting.m_nSlots)
	{
	case 4:
		SendDlgItemMessage(IDC_COMBO_NPOS, CB_SETCURSEL, (WPARAM)COMBO_4_SLOTS);
		break;
	case 8:
		SendDlgItemMessage(IDC_COMBO_NPOS, CB_SETCURSEL, (WPARAM)COMBO_8_SLOTS);
		break;
	case 12:
		SendDlgItemMessage(IDC_COMBO_NPOS, CB_SETCURSEL, (WPARAM)COMBO_12_SLOTS);
		break;
	case 24:
		SendDlgItemMessage(IDC_COMBO_NPOS, CB_SETCURSEL, (WPARAM)COMBO_24_SLOTS);
		break;
	}
	
	
	// if m_CurrentSetting.m_strCurrentCamFriendlyName have a valid Camera Name then choose it.
	CString CamName = m_CurrentSetting.m_strCurrentCamFriendlyName;
	if (CamName != EMPTY)
	{
		int idx = SendDlgItemMessage(IDC_COMBO_CAM, CB_FINDSTRING, -1, (LPARAM)(LPCTSTR)CamName);
		if(idx != CB_ERR)
			SendDlgItemMessage(IDC_COMBO_CAM, CB_SETCURSEL, (WPARAM)idx);
	}
	else
	{
		// Set the first choice as the default camera
		SendDlgItemMessage(IDC_COMBO_CAM, CB_SETCURSEL, (WPARAM)0);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// for now populate only the camera device combo-box
void CSettingDlg::PopulateDlg(const std::vector<sCamera>& CamList)
{
	for (auto Cam : CamList)
		SendDlgItemMessage(IDC_COMBO_CAM, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)Cam.strName);
}
