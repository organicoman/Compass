#pragma once
#include "Symbols.h"
#include "CameraWnd.h"

// CCarouselWnd frame

class CCarouselWnd : public CFrameWndEx
{
	DECLARE_DYNCREATE(CCarouselWnd)
public:
	CCarouselWnd();           
	virtual ~CCarouselWnd();

	//Embeded controles
protected:
	CMFCToolBar m_toolbar;
	CCameraWnd* m_Camera = nullptr;
	BOOL fCamOnOff = FALSE;

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewButtonClicked();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

