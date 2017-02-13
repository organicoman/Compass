#pragma once

#include "afxdialogex.h"
#include "Utility/VideoCapture.h" // just a Utility to populate this dialog box with the available cams


// CSettingDlg dialog
class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(BOOL bValidRegistry, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void PopulateDlg(const std::vector<sCamera>& CamList);
	

public:
	// Holds the list of all Video Capture devices on the Machine.
	std::vector<sCamera> m_CamList;

	// a Copy of the User's Last Saved Settings (previous run).
	sProfile m_CurrentSetting;
};
