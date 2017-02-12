// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Compass.h"
#include "SettingDlg.h"
#include <cstdlib>

#define COMBO_4_SLOTS 0
#define COMBO_8_SLOTS 1
#define COMBO_12_SLOTS 2
#define COMBO_24_SLOTS 3

// CSettingDlg dialog

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CString AppDirectory, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTINGDLG, pParent)
{
	// manage the xml config file.

	// if the App failed to create the AppDirectory Folder, retry again here.
	if (AppDirectory == _T("."))
	{
		CString CompassDirectory;

		BOOL err = CompassDirectory.GetEnvironmentVariable(_T("LOCALAPPDATA")); //on failure use the "\\Temp" folder

		if (!err)
			CompassDirectory.GetEnvironmentVariable(_T("TEMP"));

		CompassDirectory = CompassDirectory + CString(_T("\\Compass"));

		// Create the Directory
		BOOL bErr = CreateDirectory(CompassDirectory, 0);
		if (bErr == ERROR_PATH_NOT_FOUND)
		{
			TRACE("Failed to Create the Application Directory Again!\n");
		}
	}
	else
	{
		m_strXmlFile = AppDirectory + CString(_T("\\cfgcompass.xml"));
		FILE* pfile;
		errno_t err = _tfopen_s(&pfile, m_strXmlFile, _T("r+"));
		if (!err)
		{
			tinyxml2::XMLError cErr = m_XmlDoc.LoadFile(pfile);
			//fill the structure sXMLData with the data from the configuration file.
			Fill_Settings(&m_XmlDoc);
			
		}
		else
		{
			// the format of the config.xml file. Should be loaded from a Resource String instead of Hard coding it here.
		    CString XmlConfig = _T("<?xml version=\"1.0\"?>\n<compass v=\"0.1\">\n\t<SlotsNumber></SlotsNumber>\n\t<CameraName></CameraName>\n\t<CameraID></CameraID>\n</compass>");
			err = _tfopen_s(&pfile, m_strXmlFile, _T("a+t"));
			if (!err)
			{
				size_t len = fwrite((const void*)XmlConfig.GetBuffer(), sizeof(_T("")), XmlConfig.GetLength(), pfile);
				fflush(pfile);
				fclose(pfile);
			}
				
			else
			{
				MessageBox(_T("Could not create the Configuration File!\nThe settings will not be saved."), 0, MB_OK | MB_ICONHAND);
				m_strXmlFile = _T("");
			}
		}
	}
}

CSettingDlg::~CSettingDlg()
{
	 //before destruction check if we need to save the new settings to the XML configuration file.
	if (m_Settings.m_bNeedUpdate)
	{
		FILE* pfile;
		errno_t err = _tfopen_s(&pfile, m_strXmlFile, _T("w"));

		tinyxml2::XMLError cErr = m_XmlDoc.LoadFile(pfile);

		tinyxml2::XMLElement* nSlots = m_XmlDoc.FirstChildElement("SlotsNumber");
		nSlots->SetText(m_Settings.m_strSlots);
		tinyxml2::XMLElement* CamNam = m_XmlDoc.FirstChildElement("CameraName");
		CamNam->SetText(m_Settings.m_strCurrentCamFriendlyName);
		tinyxml2::XMLElement* CamID = m_XmlDoc.FirstChildElement("CameraID");
		CamID->SetText(m_Settings.m_strCurrentCamPath);
		//finally save all these changes in the memory XMLDoc to Disk.
		m_XmlDoc.SaveFile(pfile);
		fclose(pfile);
	}
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// Capture the Selection and push it to the App config file

	CString FriendlyName;
	CString nSlots;
	if (pDX->m_bSaveAndValidate)
	{
		DDX_CBString(pDX, IDC_COMBO_CAM, FriendlyName);
		m_Settings.m_strCurrentCamFriendlyName = FriendlyName;
		
		int idx = SendDlgItemMessage(IDC_COMBO_CAM, CB_GETCURSEL);
		m_Settings.m_strCurrentCamPath = m_CamList[idx].strID; //save the pathID of selected camera

		DDX_CBString(pDX, IDC_COMBO_NPOS, nSlots);
		m_Settings.m_strSlots = nSlots;
		
		m_Settings.m_bNeedUpdate = TRUE;
		return;
	}
	
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
END_MESSAGE_MAP()

// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// First Populate this Structure with the list of available Video Capture devices on this machine.

LOOP:
	if (EnumVideoDev(&m_CamList) == S_FALSE)
	{
		int nResponse = MessageBox(_T("Unable to Enumerate the Video Capture Devices on this Machine!\n"\
			"if you choose Continue, No Camera will be Available in the Camera Screen"), _T("ERROR"),
			MB_ICONEXCLAMATION | MB_CANCELTRYCONTINUE);
		if (nResponse == IDRETRY)
			goto LOOP; //this loop could be changed to a Do-While{} loop. Same thing...Tremendous!
		else
		{
			MessageBox(_T("No Camera Was Selected!"), 0, MB_OK);
			SendDlgItemMessage(IDC_COMBO_CAM, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)_T("NO CAMERA AVAILABLE"));
			SendDlgItemMessage(IDC_COMBO_CAM, CB_SETCURSEL,0);
			m_Settings.m_bNeedUpdate = TRUE; // in case the camera saved previously was removed in this run.
			return TRUE;
		}
	}

	// Otherwise
	PopulateDlg(m_CamList); // fill the Cam Combo Box with available Cameras FriendlyNames.

	// Set the Number of Slots on the carousel to 8 by default
	SendDlgItemMessage(IDC_COMBO_NPOS, CB_SETCURSEL, (WPARAM)COMBO_8_SLOTS);
	// Set the first choice as the default camera
	SendDlgItemMessage(IDC_COMBO_CAM, CB_SETCURSEL, (WPARAM)0);

	 //if the Xml.config file exist on this Machine,then match the current selections of all the Combo-Boxes.
	if (m_Settings.m_bFromFile)
	{
		CString nSlot = m_Settings.m_strSlots;
		CString CamID = m_Settings.m_strCurrentCamPath;
		
		int idx_slot = SendDlgItemMessage(CB_SELECTSTRING, IDC_COMBO_NPOS, (WPARAM)-1, (LPARAM)(LPCTSTR)nSlot);
		if (idx_slot == CB_ERR)
		{
			TRACE("The Selected number of Slots is not Found!\n");
			SendDlgItemMessage(CB_SETCURSEL, IDC_COMBO_NPOS, (WPARAM)COMBO_8_SLOTS);
		}

		int idx_cam = SendDlgItemMessage(CB_SELECTSTRING, IDC_COMBO_CAM, (WPARAM)-1, (LPARAM)(LPCTSTR)CamID);
		if (idx_cam == CB_ERR) //in case the camera was removed
		{
			TRACE("Couldn't find the Camera FriendlyName in the List\n");
			MessageBox(_T("The Camera used in the last session was removed\n, Falling-back to the First choice in the list?"), 0, MB_OK | MB_ICONEXCLAMATION);
			SendDlgItemMessage(CB_SETCURSEL, IDC_COMBO_CAM, (WPARAM)0);
		}
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

void CSettingDlg::Fill_Settings(tinyxml2::XMLDocument * XmlDoc)
{
	tinyxml2::XMLElement* numSlots = XmlDoc->FirstChildElement("SlotsNumber");
	if (numSlots)
	{
		const char* n = numSlots->GetText();
		m_Settings.m_strSlots = CString(n);
		m_Settings.m_bFromFile = TRUE;
	}
	
	tinyxml2::XMLElement* CamNam = XmlDoc->FirstChildElement("CameraName");
	if (CamNam)
	{
		const char* name = CamNam->GetText();
		m_Settings.m_strCurrentCamFriendlyName = CString(name);
		m_Settings.m_bFromFile = TRUE;
	}

	tinyxml2::XMLElement* CamID = XmlDoc->FirstChildElement("CameraID");
	if (CamID)
	{
		const char* id = CamID->GetText();
		m_Settings.m_strCurrentCamPath = CString(id);
		m_Settings.m_bFromFile = TRUE;
	}
}
