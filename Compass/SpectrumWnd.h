#pragma once
#include "Symbols.h"

// CSpectrumWnd frame

class CSpectrumWnd : public CFrameWndEx
{
	DECLARE_DYNCREATE(CSpectrumWnd)
public:
	CSpectrumWnd();           
	virtual ~CSpectrumWnd();

	//Embeded controles
protected:
	CMFCToolBar m_toolbar;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void PostNcDestroy();
};


