#pragma once

#include "Prpty_res.h"
#include "QuantificationPage.h"
#include "ElementPage.h"
#include "AcquiDisplPage.h"

// CPropertyWnd

class CPropertyWnd : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertyWnd)

public:
	CPropertyWnd();
	CPropertyWnd(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropertyWnd();
	void AdjustPagesLayout();

protected:
	DECLARE_MESSAGE_MAP()

	//member variables of the PropertySheet (Quantification, ElementID, Acquisition, Display)
private:
	CQuantificationPage pageQuant;
	CElementPage pageElemt;
	CAcquiDisplPage pageAcqui_Displ;
	virtual void PostNcDestroy();
};


