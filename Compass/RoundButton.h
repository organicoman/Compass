#pragma once

// CRoundButton

class CRoundButton : private CMFCButton
{
	DECLARE_DYNAMIC(CRoundButton)
	enum STATE{ST_DISABLED, ST_ENABLED};

public:
	CRoundButton();
	virtual ~CRoundButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnDraw(CDC* pDC, const CRect& rect, UINT uiState);
	virtual BOOL Create(LPCTSTR lpszCaption, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
private:
	CRgn m_CircleRgn;
	STATE m_State;

};


