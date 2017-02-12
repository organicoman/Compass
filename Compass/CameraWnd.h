#pragma once
#include "Symbols.h"
#include "Utility/VideoCapture.h"

// CCameraWnd frame

class CCameraWnd : public CFrameWndEx
{
	DECLARE_DYNCREATE(CCameraWnd)
public:
	CCameraWnd();
	          
	virtual ~CCameraWnd();
	
	//Embeded controles
protected:
	CMFCToolBar m_toolbar;
	CameraObj* m_VideoCam;
	std::vector<sCamera> m_CamList;
	
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
		                UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


