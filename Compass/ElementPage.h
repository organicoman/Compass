#pragma once
#include "Prpty_res.h"


// CElementPage dialog

class CElementPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CElementPage)

public:
	CElementPage();
	virtual ~CElementPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_ELMNTS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
