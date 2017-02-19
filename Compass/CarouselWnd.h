#pragma once
#include "Symbols.h"
#include "CameraWnd.h"

// all this can be loaded from a theme file if you want, Including the CPen and CBrush member objects of the Class down here.
const COLORREF cDisabled = RGB(80, 80, 80);
const COLORREF cDisabledPen = RGB(10, 10, 10);

const COLORREF cActive = RGB(0, 160, 0);
const COLORREF cActivePen = RGB(0, 90, 50);

const COLORREF cSelected = RGB(0, 190, 0);
const COLORREF cSelectedPen = RGB(230, 0, 0);

const COLORREF cHotLit = RGB(0, 140, 200);
const COLORREF cHotLitPen = RGB(0, 80, 200);


// CCarouselWnd frame
// The purpose of the Class is to draw it's items and manage their visual states. Namely the Carousel figure. and communicate
// the indexes of the added slots to the outside world

class CCarouselWnd : public CFrameWndEx
{
	DECLARE_DYNCREATE(CCarouselWnd)

	// Helper Structure //
	struct sSlot
	{
		CRect rcHitRgn;
		enum SLOT_STATE { SLOT_DISABLED, SLOT_ENABLED, SLOT_SELECTED, SLOT_ADDED, SLOT_ADDED_SELECTED };
		SLOT_STATE enmState; // assign Value from the above enum to choose visual appearance
		BOOL bChanged;  // turn this to FALSE to prevent visual update
	};
	//////////////////////

public:
	explicit CCarouselWnd(UINT nSlots);           
	virtual ~CCarouselWnd();
protected:
	CCarouselWnd(); // this Constructor is not to be called.
	//Embedded controls
protected:
	CCameraWnd* m_Camera = nullptr;
	BOOL fCamOnOff = FALSE;
	CMFCButton m_CamBtn;
	CMFCButton m_AddBtn;
protected:
	DECLARE_MESSAGE_MAP()

private: // helper functions
	void _CleanUp();
	void _SetRect(CRect& rect);
	void _DrawSlot(UINT pos, CPaintDC* pDC, CRect* boundingBox) const;
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewButtonClicked();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	// member variables
protected:
	UINT m_nSlots;
	std::vector<sSlot> m_SlotCollection;
	std::vector<UINT> m_AddedSlots;

	// Drawing Brushes and Pens
	CPen DisabledPen{ PS_SOLID,2,cDisabledPen };
	CBrush DisabledBrush{ cDisabled };

	CPen EnabledPen{ PS_SOLID, 2, cActivePen };
	CBrush EnabledBrush{ cActive };

	CPen SelectedPen{ PS_SOLID, 3, cSelectedPen };
	CBrush SelectedBrush{ cSelected };

	CPen HotLitPen{ PS_SOLID, 3, cHotLitPen };
	CBrush HotLitBrush{ cHotLit };

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonAdd();
	// Interface
public:

	// Returns the list of the Added slots (slots added several times included)
	std::vector<UINT> GetSelection() const;

	// Removes the list's items from the collection of Added slots. Called from the App dialog.(CCompassDlg).
	void OnButtonRemove(std::vector<UINT>& list);
	
};

