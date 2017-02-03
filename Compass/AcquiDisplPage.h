#pragma once
#include "Prpty_res.h"

// CAcquiDisplPage dialog

class CAcquiDisplPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CAcquiDisplPage)

public:
	CAcquiDisplPage();
	virtual ~CAcquiDisplPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_ACQUI_DISPL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
