#pragma once
#include "Symbols.h"

// CCameraWnd frame

class CCameraWnd : public CFrameWndEx
{
	DECLARE_DYNCREATE(CCameraWnd)
public:
	enum MODE { WINDOW_MODE, VIEW_MODE };
	CCameraWnd();
	CCameraWnd(MODE enmMode);           
	virtual ~CCameraWnd();
	
	//Embeded controles
protected:
	CMFCToolBar m_toolbar;
	MODE		m_ViewMode = CCameraWnd::VIEW_MODE;

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
		                UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


