#pragma once

#include <dshow.h>
#include <d3d9.h>
#include <vmr9.h>
#include <sal.h>

#include <vector>


#ifdef DLL
#define _API_ __declspec(dllexport)
#endif
#define _API_

/*
// this file contain helper objects and functions to detect Video Capture devices, and Preview Video in a Window
// The Versions of Windows supported are: Windows Vista and later for EVR (Desktop only)---Not Implemented yet!
// or Windows XP SP2 and later for VMR9 (Desktop only)
// All the functions prefixed with "_API_" are API functions and can be used alone.
// The User is encouraged to build this file as a Dll, then use the functions below as a set of API.
// Important: add "/DDLL" directive when you build it.
*/

//Helper object to store the device name and ID
struct sCamera
{
	CString strName;
	CString strID;
};

_API_ HRESULT __cdecl EnumVideoDev(_Inout_ std::vector<sCamera>* buf);

_API_ class CameraObj
{
public:
	enum RETURNCODE{RETURN_SUCCESS=0, RETURN_UNABLE_TO_CONNECT, RETURN_CAMERA_REMOVED, RETURN_ERROR =-1};

public:
	// the CameraObj can be Attached to One on Only One CWnd*, which must not be a null pointer.
	explicit CameraObj(const CWnd* parent);

	// the Output Video will respect the Original Aspect ratio inside the "destRect" argument.
	CameraObj(const CWnd* parent, CRect destRect);

	virtual ~CameraObj();

protected:
	CameraObj() {}; //this object must be constructed with a window container, thus protecting the default constructor.

public:
	//interface

	// Returns a list of available camera on the machine, 
	// use "buf.strName" for UI purpose, "buf.strID" as a unique criteria of selection.
	// this function is totally self encapsulated (i.e doesn't create the filterGraph)
	RETURNCODE EnumerateCams(std::vector<sCamera>& buf) const;

	// initialize the COM library, look for the camera using it's "camera.strID" and build the filterGraph object.
	// Closing the Com library is done in the destructor of the object instance.
	RETURNCODE ConnectToCam(const CString camPath);

	// After the Cam was Connected, 
	// Call this method to build the filterGraph and start streaming video(the Aspect Ratio is respected).
	RETURNCODE Run();

	// Pauses the Graph if it is in Running state.
	CameraObj::RETURNCODE Pause() const;

	// Resumes Playing after a Pause/Stop operation
	CameraObj::RETURNCODE Play() const;

	// Get the State of the filterGraph;(0 = STOP/PAUSED, 1 = RUNNING, anything else is error).
	int GetCurrentRunState()const;

	// Get Ideal Image size and it's Aspect Ratio.
	CameraObj::RETURNCODE GetNativeVideoDim(LONG& width, LONG& height, LONG& ARwidth, LONG& ARheight) const;

	// Update the Size of the Output rectangle if the container Window changes Size.
	CameraObj::RETURNCODE UpdateSize();

	CameraObj::RETURNCODE CaptureImage(CBitmap& imageBuf) const;
	//RETURNCODE RecordVideo(const CString& filePath);
	//RETURNCODE StopRecord();
	//RETURNCODE WriteText(const CString& text, const CPoint pos, const CFont* font = nullptr, const COLORREF color = RGB(0, 0, 0)/*BLACK*/);
	RETURNCODE DrawGraticul(const CPoint center, const CSize size, BOOL DimScreen = TRUE);
	//RETURNCODE DrawRectangle(const CPoint pos, const CSize size);
	//RETURNCODE AddWaterMark(const CImage bitmap);
	const CWnd* GetContainerWnd()const { return m_containerWnd; };

private:
	HRESULT _InitializeCOM();
	void    _CleanCOM();
	CameraObj::RETURNCODE _BuildGraph();
	CameraObj::RETURNCODE _BuildCaptureGraph();
	CameraObj::RETURNCODE _ConnectBuilderToCapture();
	CameraObj::RETURNCODE _AddCameraFilter();
	CameraObj::RETURNCODE _AddVMR9filter();
	CameraObj::RETURNCODE _ConfigVMR9();
	CameraObj::RETURNCODE _StartGraph();

private:
	IMoniker* m_camMoniker;
	IGraphBuilder* m_filterGraph;
	ICaptureGraphBuilder2* m_captureGraph;
	IBaseFilter* m_windowlessVMRfilter;

	const CWnd* m_containerWnd;
	CRect m_viewRect;
	//BOOL m_bRecording;
	static BOOL m_bIsCOM ;  // shared between all instances of this Object.
};
