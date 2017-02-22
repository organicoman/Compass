#pragma once


// CGraticule

class CGraticule : public CStatic
{
	DECLARE_DYNAMIC(CGraticule)

public:
	CGraticule();
	virtual ~CGraticule();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CRect m_rcSize;

public:
	virtual BOOL Create(CWnd* pParentWnd, const RECT& rect);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


