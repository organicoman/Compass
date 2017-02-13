// CompassDlg.h : header file
//
#pragma once
#include "CompassMainFrame.h"

// Some Helper structure needed only for this project
struct sData
{
	UINT spot;
	UINT pos;
	CString name = _T("Untitled");
	double kV;
	double mA;
	double cs;
	CString filter;
	CString atmoshpere;
	CString path;
	CString preset;
	int    capture;
};

struct sTable
{
	//use vector's built-in methods for elements manipulation, use Getters/Setter for data memebers manipulation.
	CArray<sData> Table;
	//Getters
	UINT GetSpot(UINT idx) const { return Table[idx].spot; }
	UINT GetPos(UINT idx) const { return Table[idx].pos; }
	CString GetName(UINT idx)const { return Table[idx].name; }
	double GetKV(UINT idx)const { return Table[idx].kV; }
	double GetMA(UINT idx)const { return Table[idx].mA; }
	double GetCS(UINT idx)const { return Table[idx].cs; }
	CString GetFilter(UINT idx)const { return Table[idx].filter; }
	CString GetAtmosphere(UINT idx)const { return Table[idx].atmoshpere; }
	CString GetPath(UINT idx)const { return Table[idx].path; }
	CString GetPreset(UINT idx)const { return Table[idx].preset; }
	int GetCapture(UINT idx)const { return Table[idx].capture; }
	// Setters
	void SetSpot(UINT idx, UINT spot) { Table[idx].spot = spot; }
	void SetPos(UINT idx, UINT pos) { Table[idx].pos = pos; }
	void SetName(UINT idx, CString name) { Table[idx].name = name; }
	void SetKV(UINT idx, double kV) { Table[idx].kV = kV; }
	void SetMA(UINT idx, double mA) { Table[idx].mA = mA; }
	void SetCS(UINT idx, double cs) { Table[idx].cs = cs; }
	void SetFilter(UINT idx, CString filter) { Table[idx].filter = filter; }
	void SetAtmosphere(UINT idx, CString atmosphere) { Table[idx].atmoshpere = atmosphere; }
	void SetPath(UINT idx, CString path) { Table[idx].path = path; }
	void SetPreset(UINT idx, CString preset) { Table[idx].preset = preset; }
	void SetCapture(UINT idx, int capture) { Table[idx].capture = capture; }
};



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
	// This member should be transfered to Carousel CDoc object for serialization
	sTable m_table;

// Helper Functions.
public:
	void SetupTable();
	void SetupIcons();

	// Helper function to Create the Main FrameWnd
	BOOL CreateCompassFrame();
	afx_msg void OnIdwCamera();
	afx_msg void OnIdwCarousel();
	afx_msg void OnIdwSpectrum();
	afx_msg void OnIdwProperty();
	afx_msg void OnButtonAdd();
private:
	// Populate the table using the controls in the Header toolbar
	void PopulateTable(sData& tmpData);

};