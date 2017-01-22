#pragma once
#include "Prpty_res.h"

// CQuantificationPage dialog

class CQuantificationPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CQuantificationPage)

public:
	CQuantificationPage();
	virtual ~CQuantificationPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_QUANT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	//Dialog Ctrl Members.
private:
	CListCtrl m_listCtrl_Compnt;
	CListCtrl m_listCtrl_Analyze;
public:
	// Adds the Headers to the Tables
	void SetupTables();
	virtual BOOL OnInitDialog();
};
