#pragma once

#include "CarouselWnd.h"
#include "CameraWnd.h"
#include "SpectrumWnd.h"
#include "PropertyWnd.h"

// CCompassMainFrame frame

class CCompassMainFrame : public CFrameWnd
{
	
public:
	CCompassMainFrame();           // public constructor/destructor no need for dynamic creation
	~CCompassMainFrame();			 // Non virtual destructor , this class will not be inherited

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Attributes
public:
	enum CURRENTWND{WND_CAROUSEL, WND_CAMERA, WND_SPECTRUM, WND_PROPERTY};

	// Private attributes
private:
	CCarouselWnd*    m_wndCarousel = nullptr;
	CCameraWnd*      m_wndCamera = nullptr;
	CSpectrumWnd*    m_wndSpectrum = nullptr;
	CPropertyWnd*    m_wndProperty = nullptr;
	CURRENTWND       m_enmCurrentWnd;
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	
	// Switch between the different screens (Camera, Carousel, Spectrum...)
	BOOL SwitchWnd(CURRENTWND enIDwnd);
protected:
	// Create and populate the Camera Wnd
	BOOL SetupCameraWnd();
	// Create and Populate the Carousel Wnd then Add it to the DynamicLayoutManager
	BOOL SetupCarouselWnd();
	// Create and populate the the Spectrum Wnd then Add it to the DynamicLayout Manger
	BOOL SetupSpectrumWnd();
	// Create and populate the the Property Wnd then Add it to the DynamicLayout Manger
	BOOL SetupPropertyWnd();
	// Deactivate then destroy the actual child window, then free the pointer resource to it.
	void KillCurrentWnd(CURRENTWND enmIDwnd);
public:
	// Returns the Actual screen window (carousel, camera, spectrum...) in the mainFrame
	CURRENTWND GetCurrentScreen() { return m_enmCurrentWnd; };
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	std::vector<UINT> GetSelectedSlots() const;
};