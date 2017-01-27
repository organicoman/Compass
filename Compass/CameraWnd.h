#pragma once
#include "Symbols.h"

// CCameraWnd frame

class CCameraWnd : public CFrameWndEx
{
	DECLARE_DYNCREATE(CCameraWnd)
public:
	CCameraWnd();           
	virtual ~CCameraWnd();
	enum MODE {WINDOW_MODE, VIEW_MODE};

	//Embeded controles
protected:
	CMFCToolBar m_toolbar;
	MODE		m_ViewMode;

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	// Gets Sets the Camera View mode (i.e is it a Window or a View in a Window)
	CCameraWnd::MODE GetMode();
	void SetMode(MODE enmMode);
};


