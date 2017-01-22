#pragma once


// CCarouselView view

class CCarouselView : public CView
{
	DECLARE_DYNCREATE(CCarouselView)

protected:
	CCarouselView();           // protected constructor used by dynamic creation
	virtual ~CCarouselView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


