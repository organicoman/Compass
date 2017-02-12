#pragma once

#include "afxdialogex.h"
#include "Utility/VideoCapture.h" // just a Utility to populate this dialog box with the available cams
#include "Utility/XMLData.h"
#include "Utility/tinyxml2.h"

// CSettingDlg dialog
class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CString AppDirectory, CWnd* pParent = NULL);   // standard constructor
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
	void Fill_Settings(tinyxml2::XMLDocument* XmlDoc);

public:
	std::vector<sCamera> m_CamList; 
	sXMLData m_Settings;
	tinyxml2::XMLDocument m_XmlDoc;
	CString m_strXmlFile;

};
