// CompassDlg.h : header file
//
#pragma once
#include "CompassMainFrame.h"

// CCompassDlg dialog
class CCompassDlg : public CDialogEx
{
// Construction
public:
	CCompassDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPASS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_ListCtrl;
	CCompassMainFrame* m_pCWndCompass;

// Helper Functions.
private:
	void SetupTable();
	void SetupIcons();

	// Helper function to Create the Main FrameWnd
	BOOL CreateCompassFrame();

	// Populate the table using the selected Slots
	void PopulateTable(std::vector<UINT>& selection);

public:
	// Message Handlers
	afx_msg void OnIdwCamera();
	afx_msg void OnIdwCarousel();
	afx_msg void OnIdwSpectrum();
	afx_msg void OnIdwProperty();
	afx_msg void OnButtonAdd();
};