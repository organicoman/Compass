
#include "stdafx.h"
#include "VideoCapture.h"

#pragma comment(lib,"Strmiids")

#define VMR9_RENDERER _T("VMR9 Renderer")
#define CAPTURE_FILTER _T("Capture Filter")

// Enumerating the Video Capt Devices on the machine.
// usage: call this function to get a Video Capt Devices list, use the buf[indx]->strName for UI display, 
//        and the buf[indx]->strID to select the device.
HRESULT EnumVideoDev(std::vector<sCamera>* buf)
{
	//this function crashs if its argument is NULL
	ICreateDevEnum* pDevEnum = NULL;
	IEnumMoniker* pDevMoniker = NULL;
	IMoniker* pMoniker = NULL;

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		TRACE("Error: Failed to Initialize the COM library! \n");
		return hr;
	}
	
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
	if (FAILED(hr))
	{
		TRACE("Failed: Couldn't Create a device enumerator interface! \n");
		return hr;
	}

	hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pDevMoniker, 0 /*enumerate everything*/);
	if (hr == S_OK)
	{
		pDevEnum->Release(); //free non needed resource.
		int idx = 0;
		while (pDevMoniker->Next(1, &pMoniker, NULL) == S_OK)
		{
			IPropertyBag* pBag = NULL;
			hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pBag));
			if (hr != S_OK)
			{
				TRACE("Unable to Bind the Moniker to a Property Bag\n");
				continue; //check the next device
			}
			VARIANT var, id;
			VariantInit(&var);
			VariantInit(&id);

			hr = pBag->Read(_T("FriendlyName"), &var, 0/*no need to record errors*/);
			if (hr != S_OK)
			{
				TRACE("the Device n#%d, doesn't have a friendly name, We will try its Description\n", idx);
				// try the Description property.
				hr = pBag->Read(_T("Description"), &var, 0);
				if (hr == S_OK)
				{
					pBag->Read(_T("DevicePath"), &id, 0); //no need to catch for error, because it's evident
					buf->emplace_back(sCamera{ CString(var.bstrVal),CString(id.bstrVal) }); //record the description as the device Name.
					goto CLR_CNT;
				}
				else
				{
					pBag->Read(_T("DevicePath"), &id, 0);
					buf->emplace_back(sCamera{ CString("Unknown Device!"),CString(id.bstrVal) });
					goto CLR_CNT;
				}
			}
			pBag->Read(_T("DevicePath"), &id, 0);//no need to check for error, because it's evident
			buf->emplace_back(sCamera{ CString(var.bstrVal),CString(id.bstrVal) }); //record the friendly name
		CLR_CNT:
			VariantClear(&var);
			VariantClear(&id);
			idx++;
			pBag->Release();
			continue; //next device
		}
		// shrink the vector size to include only its items(memory optimization)
		buf->shrink_to_fit();
		pMoniker->Release();
		pDevMoniker->Release();
		CoUninitialize();
		return S_OK;
	}
	else
	{
		TRACE("Failed to Create Video Capture Class Enumerator\n");
	}
	pDevEnum->Release(); //free non needed resource.
	CoUninitialize();
	return S_FALSE;
}

///////////////////////
// The Camera Object //
///////////////////////

BOOL CameraObj::m_bIsCOM = FALSE;

CameraObj::CameraObj(const CWnd * parent):
	m_containerWnd(parent), m_camMoniker(nullptr), m_captureGraph(nullptr), m_filterGraph(nullptr), 
	m_windowlessVMRfilter(nullptr), m_viewRect(CRect{ 0,0,0,0 })
{
	
	ASSERT(parent);
	if (parent == nullptr)
	{
		MessageBox(NULL, _T("FAILURE: Trying to create a Camera without a Camera Window Container!"), _T("CameraObj::CameraObj- ERROR"), MB_OK | MB_ICONSTOP);
		this->~CameraObj(); // calling the destructor from inside the constructor to prevent the instance from existing!!..pretty smart huh?
	}
}

CameraObj::CameraObj(const CWnd * parent, CRect destRect):
	m_containerWnd(parent), m_camMoniker(nullptr), m_captureGraph(nullptr), m_filterGraph(nullptr), m_windowlessVMRfilter(nullptr),
	m_viewRect(destRect)
{
	
	ASSERT(parent);
	if (parent == nullptr)
	{
		MessageBox(NULL, _T("FAILURE: Trying to create a Camera without a Camera Window Container!"), _T("CameraObj::CameraObj- ERROR"), MB_OK | MB_ICONSTOP);
		this->~CameraObj(); // calling the destructor from inside the constructor to prevent the instance from existing!!..pretty smart huh?
	}
}

CameraObj::~CameraObj()
{
	_CleanCOM();
	if (m_bIsCOM)
	{
		CoUninitialize();
		m_bIsCOM = FALSE;
	}
}

CameraObj::RETURNCODE CameraObj::EnumerateCams(std::vector<sCamera>& buf) const
{
	HRESULT hr = EnumVideoDev(&buf);
	if (hr == S_OK)
		return RETURN_SUCCESS;

	return RETURN_ERROR;
}

CameraObj::RETURNCODE CameraObj::ConnectToCam(const CString camPath)
{
	ICreateDevEnum* pDevEnum = NULL;
	IEnumMoniker* pDevMoniker = NULL;

	if (!m_bIsCOM)
		_InitializeCOM(); // let's bet that this method return S_OK. Potential risk: several instances of the COM library open.

    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
	if (hr!=S_OK)
	{
		TRACE("CameraObj::ConnecToCam- FAILED: Couldn't Create a device enumerator interface! \n");
		return RETURN_ERROR;
	}

	hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pDevMoniker, 0 /*enumerate everything*/);
	if (hr != S_OK)
	{
		TRACE("CameraObj::ConnectToCam- ERROR: while enumerating Monikers!\n");
		pDevEnum->Release();
		return RETURN_ERROR;
	}

	pDevEnum->Release(); // Not needed

	while (pDevMoniker->Next(1, &m_camMoniker, NULL) == S_OK)
	{
		IPropertyBag* pBag = nullptr;
		hr = m_camMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pBag));
		if (hr != S_OK)
		{
			TRACE("CameraObj::ConnectToCam- ERROR: Failed while binding the Moniker to the Property Bag\n");
			m_camMoniker->Release();
			continue;
		}
		//check against the argument data
		VARIANT var;
		VariantInit(&var);

		hr = pBag->Read(_T("DevicePath"), &var, 0); //Guaranteed to be Unique for each device.
		if (hr != S_OK)
		{
			TRACE("CameraObj::ConnectToCam- ERROR: while reading the DevicePath Property.\n");
			m_camMoniker->Release();
			pBag->Release();
			continue; //next device.
		}
		CString temp(var.bstrVal);
		if (!temp.Compare(camPath)) // returns 0 on identity.
		{
			VariantClear(&var);
			pBag->Release();
			pDevMoniker->Release();
			return RETURN_SUCCESS; // keep this instance of m_camMoniker, it will be used in subsequent operations
		}
		//Not what we are looking for... Next device.
		pBag->Release();
		m_camMoniker->Release();
	}
	// No device match was found
	// free everything.
	TRACE("CameraObj::ConnectToCam- Failed to find the selected Camera... Maybe it was removed!\n");
	pDevMoniker->Release();
	m_camMoniker->Release();
	m_camMoniker = nullptr;
	
	return RETURN_CAMERA_REMOVED;
}

HRESULT CameraObj::_InitializeCOM()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (hr == RPC_E_CHANGED_MODE)
	{
		TRACE("CameraObj: ERROR- Could not Initialize the COM Library!\n");
		return hr;
	}
	
	//even if the COM library was initialized some where else in this thread, 
	//this CameraObj Instance will be responsible to close its respective COM instance (ref decrement).
	m_bIsCOM = TRUE; 
	return hr;
}

void CameraObj::_CleanCOM()
{
	if (m_windowlessVMRfilter)
	{
		m_windowlessVMRfilter->Release();
		m_windowlessVMRfilter = nullptr;
	}
	if (m_captureGraph)
	{
		m_captureGraph->Release();
		m_captureGraph = nullptr;
	}
	if (m_filterGraph)
	{
		m_filterGraph->Release();
		m_filterGraph = nullptr;
	}
	if (m_camMoniker)
	{
		m_camMoniker->Release();
		m_camMoniker = nullptr;
	}
}

CameraObj::RETURNCODE CameraObj::_BuildGraph()
{
	if (m_filterGraph == nullptr)
	{
		HRESULT hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_filterGraph);
		if (hr != S_OK)
		{
			TRACE("CameraObj::Run- FAILURE: couldn't create the FilterGraph!\n");
			return RETURN_ERROR;
		}
	}
	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::_BuildCaptureGraph()
{
	if (m_captureGraph == nullptr)
	{
		HRESULT hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_captureGraph));
		if (hr != S_OK)
		{
			TRACE("CameraObj::Run- FAILURE: unable to create the CaptureGraph!\n");
			_CleanCOM();
			return RETURN_ERROR;
		}
	}
	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::_ConnectBuilderToCapture()
{
	ASSERT(m_filterGraph && m_captureGraph);
	HRESULT hr = m_captureGraph->SetFiltergraph(m_filterGraph);
	if (hr != S_OK)
	{
		TRACE("CameraObj::Run- FAILURE: unable to connect the FilterGraph to the CaptureGraph\n");
		_CleanCOM();
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::_AddCameraFilter()
{
	ASSERT(m_camMoniker && m_filterGraph && m_captureGraph);
	
	IBaseFilter* pBase = nullptr;
	HRESULT hr = m_camMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pBase);
	if (hr == S_OK)
	{
		hr = m_filterGraph->AddFilter(pBase, CAPTURE_FILTER);
		if (hr != S_OK)
		{
			TRACE("CameraObj::Run- Failed Adding the Capture Filter.\n");
			_CleanCOM();
			return RETURN_ERROR;
		}
		return RETURN_SUCCESS;
	}
	TRACE("CameraObj::Run- FAILURE: unable to Connect to the Camera.\n");
	_CleanCOM();
	return RETURN_UNABLE_TO_CONNECT;
}

CameraObj::RETURNCODE CameraObj::_AddVMR9filter()
{
	ASSERT(m_filterGraph && m_captureGraph);

	HRESULT hr;
	if (m_windowlessVMRfilter == nullptr)
	{
		hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_windowlessVMRfilter));
		if (hr != S_OK)
		{
			TRACE("CameraObj::Run- FAILURE: unable to create the VMR9 renderer!\n");
			_CleanCOM();
			return RETURN_ERROR;
		}
	}
	// Add it to the Graph
	hr = m_filterGraph->AddFilter(m_windowlessVMRfilter, VMR9_RENDERER);
	if (hr != S_OK)
	{
		TRACE("CameraObj::Run- FAILURE: unable to add a VMR9 Renderer to the FilterGraph!\n");
		_CleanCOM();
		return RETURN_ERROR;
	}
	
	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::_ConfigVMR9()
{
	ASSERT(m_windowlessVMRfilter);
	if (m_windowlessVMRfilter == nullptr)
	{
		RETURNCODE ret = _AddVMR9filter();
		if (ret != RETURN_SUCCESS)
			return ret;
	}

	IVMRFilterConfig9* pVMR9config = nullptr;
	IVMRWindowlessControl9* pVMR9control = nullptr;
	
	// Query these interfaces
	HRESULT hr = m_windowlessVMRfilter->QueryInterface(IID_IVMRFilterConfig9, (void**)&pVMR9config);
	if (hr != S_OK)
	{
		TRACE("CameraObj::Run- FAILED: to Query the FilterConfig9\n");
		_CleanCOM();
		return RETURN_ERROR;
	}
	pVMR9config->SetRenderingMode(VMR9Mode_Windowless);
	hr = m_windowlessVMRfilter->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pVMR9control);
	if (hr != S_OK)
	{
		TRACE("CameraObj::Run- FAILED: to Query the WindowlessControl9\n");
		pVMR9config->Release();
		_CleanCOM();
		return RETURN_ERROR;
	}
	// windowless mode with CWnd* as a container, display in CRect
	hr = pVMR9control->SetVideoClippingWindow(m_containerWnd->GetSafeHwnd());
	
	// we should always respect the aspect ratio of the native video capture.
	hr = pVMR9control->SetAspectRatioMode(VMR9ARMode_LetterBox);

	if (m_viewRect == CRect{ 0,0,0,0 })
	{
		m_containerWnd->GetClientRect(&m_viewRect);
	}
	
	hr = pVMR9control->SetVideoPosition(NULL, m_viewRect);
	
	// Release Resources
	pVMR9config->Release();
	pVMR9control->Release();
	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::_StartGraph()
{
	ASSERT(m_filterGraph);
	if (!m_filterGraph)
	{
		TRACE("CameraObj::RUN- ERROR: No filterGraph is Available!\n");
		return RETURN_ERROR;
	}
	
	IMediaControl* pMediaCtrl = nullptr;
	
	HRESULT hr = m_filterGraph->QueryInterface(IID_IMediaControl, (void**)&pMediaCtrl);
	if (hr != S_OK)
	{
		TRACE("CameraObj::Run- ERROR: couldn't query the MediaControl interface!\n");
		return RETURN_ERROR;
	}
	pMediaCtrl->Run();
	//Free resource
	pMediaCtrl->Release();

	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::Run()
{
	//Cannot Run if there is no Camera Moniker available, or the COM library is not Initialized.
	ASSERT(m_camMoniker && m_bIsCOM);

	if (m_camMoniker == nullptr && m_bIsCOM)
		return RETURN_UNABLE_TO_CONNECT;

	RETURNCODE ret;
	// Start by Building the Graph Manager.
	ret = _BuildGraph();
	if (ret != RETURN_SUCCESS)
		return ret;

	// Then the Capture Graph Manager.
	ret = _BuildCaptureGraph();
	if (ret != RETURN_SUCCESS)
		return ret;

	// Connect the Capture Graph to the Filter Graph
	ret = _ConnectBuilderToCapture();
	if (ret != RETURN_SUCCESS)
		return ret;

	// Let's Start Adding Filters,
	// 1st Capture Filter.
	ret = _AddCameraFilter();
	if (ret != RETURN_SUCCESS)
		return ret;
	// 2nd VMR9 Render Filter.
	ret = _AddVMR9filter();
	if (ret != RETURN_SUCCESS)
		return ret;

	// Before running the Graph, we need to configure the VMR9 renderer to show inside our Parent Window.
	ret = _ConfigVMR9();
	if (ret != RETURN_SUCCESS)
		return ret;

	// Now everything is set Let's start streaming video.
	IBaseFilter* p_capFilter = nullptr;
	m_filterGraph->FindFilterByName(CAPTURE_FILTER, &p_capFilter);
	m_captureGraph->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, p_capFilter, NULL, m_windowlessVMRfilter);

	// Start the Graph.
	ret = _StartGraph();
	if (ret != RETURN_SUCCESS)
		return ret;

	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::Pause() const
{
	ASSERT(m_filterGraph);
	if (!m_filterGraph)
	{
		TRACE("CameraObj::Pause- ERROR: No filterGraph is Available!\n");
		return RETURN_ERROR;
	}

	IMediaControl* pMediaCtrl = nullptr;
	
	HRESULT hr = m_filterGraph->QueryInterface(IID_IMediaControl, (void**)&pMediaCtrl);
	if (hr != S_OK)
	{
		TRACE("CameraObj::Pause- ERROR: couldn't query the MediaControl Interface\n");
		return RETURN_ERROR;
	}
	FILTER_STATE fs;
	hr = pMediaCtrl->GetState(5000,(OAFilterState*) &fs);
	if (fs == State_Running)
	{
		pMediaCtrl->Pause();
		pMediaCtrl->Release();
	}
	
	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::Play() const
{
	ASSERT(m_filterGraph);
	if (!m_filterGraph)
	{
		TRACE("CameraObj::Play- ERROR: No filterGraph is Available!\n");
		return RETURN_ERROR;
	}

	IMediaControl* pMediaCtrl = nullptr;
	
	HRESULT hr = m_filterGraph->QueryInterface(IID_IMediaControl, (void**)&pMediaCtrl);
	if (hr != S_OK)
	{
		TRACE("CameraObj::Play- ERROR: couldn't query the MediaControl Interface\n");
		return RETURN_ERROR;
	}
	FILTER_STATE fs;
	hr = pMediaCtrl->GetState(5000, (OAFilterState*)&fs);
	if (fs == State_Paused || fs == State_Stopped)
	{
		hr = pMediaCtrl->Run();
		if (hr != S_OK)
		{
			TRACE("CameraObj::Play- FAILURE: Could not resume Playing!\n");
			pMediaCtrl->Release();
			return RETURN_ERROR;
		}
		pMediaCtrl->Release();
	}

	return RETURN_SUCCESS;
}

int CameraObj::GetCurrentRunState() const
{
	ASSERT(m_filterGraph);
	if (!m_filterGraph)
	{
		TRACE("CameraObj::GetCurrentState- ERROR: No filterGraph is Available!\n");
		return RETURN_ERROR;
	}
	FILTER_STATE fs;
	IBaseFilter* pVMR9filter = nullptr;
	HRESULT hr = m_filterGraph->FindFilterByName(VMR9_RENDERER, &pVMR9filter);
	if (hr != S_OK)
	{
		TRACE("CameraObj::GetCurrentState- FAILRUE: Unable to Get the VMR9 filter.\n");
		return -1;
	}
	hr = pVMR9filter->GetState(5000, &fs);
	if (fs == State_Paused || fs == State_Stopped)
		return 0;
	if (fs == State_Running)
		return 1;
	return -1;
}

CameraObj::RETURNCODE CameraObj::GetNativeVideoDim(LONG & width, LONG & height, LONG & ARwidth, LONG & ARheight) const
{
	ASSERT(m_windowlessVMRfilter);
	if (!m_windowlessVMRfilter)
	{
		TRACE("CameraObj::GetNativeVideoDim- ERROR: Windowless Renderer Filter must not be NULL.\n");
		return RETURN_ERROR;
	}
	IVMRWindowlessControl9* pVMR9ctrl = nullptr;
	HRESULT hr = m_windowlessVMRfilter->QueryInterface(IID_IVMRWindowlessControl9, (LPVOID*)&pVMR9ctrl);
	if (hr != S_OK)
	{
		TRACE("CameraObj::GetNativeVideoDim- FAILURE: unable to get the VMR9 controller\n");
		return RETURN_ERROR;
	}
	hr = pVMR9ctrl->GetNativeVideoSize(&width, &height, &ARwidth, &ARheight);
	
	pVMR9ctrl->Release();

	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::UpdateSize()
{
	if (!m_windowlessVMRfilter)
	{
		TRACE("CameraObj::UpdateSize- ERROR: Windowless Renderer Filter must not be NULL.\n");
		return RETURN_ERROR;
	}
	IVMRWindowlessControl9* pVMR9ctrl = nullptr;
	HRESULT hr = m_windowlessVMRfilter->QueryInterface(IID_IVMRWindowlessControl9, (LPVOID*)&pVMR9ctrl);
	if (hr != S_OK)
	{
		TRACE("CameraObj::UpdateSize- FAILURE: unable to get the VMR9 controller\n");
		return RETURN_ERROR;
	}
	
	m_containerWnd->GetClientRect(&m_viewRect);
	
	hr = pVMR9ctrl->SetVideoPosition(NULL, m_viewRect);

	HWND hWnd = m_containerWnd->GetSafeHwnd();
	HDC hdc = GetDC(hWnd);

	hr = pVMR9ctrl->RepaintVideo(hWnd, hdc);
	DeleteDC(hdc);
	
	pVMR9ctrl->Release();

	return RETURN_SUCCESS;
}

CameraObj::RETURNCODE CameraObj::CaptureImage(CBitmap & imageBuf) const
{
	ASSERT(&imageBuf);
	if (&imageBuf == 0)
	{
		TRACE("CameraObj::CaptureImage- ERROR: the pointer to the object that holds the Image is NULL!\n");
		return RETURN_ERROR;
	}
	// Get Access to the VMR9 renderer.
	ASSERT(m_filterGraph);
	if (!m_filterGraph)
	{
		TRACE("CameraObj::CaptureImage- ERROR: No filterGraph is Available!\n");
		return RETURN_ERROR;
	}
	IVMRWindowlessControl9* pVMR9ctrl = nullptr;
	HRESULT hr = m_filterGraph->QueryInterface(IID_IVMRWindowlessControl9, (void**)& pVMR9ctrl);
	if (hr != S_OK)
	{
		TRACE("CameraObj::CaptureImage- FAILED: couldn't query the renderer Interface!\n");
		return RETURN_ERROR;
	}

	BYTE* pDIB = nullptr;
	hr = pVMR9ctrl->GetCurrentImage(&pDIB);
	if (hr != S_OK)
	{
		TRACE("CameraObj::CaptureImage- FAILURE: couldn't capture the current image!\n");
		pVMR9ctrl->Release();
		return RETURN_ERROR;
	}
	BITMAPINFOHEADER* pBmpInfoHdr = reinterpret_cast<BITMAPINFOHEADER*>(pDIB);
	// two cases are present - Uncompressed Image, -Compressed Image.
	if (pBmpInfoHdr->biCompression == BI_RGB)
	{
		// we will capture only 24bpp and above.
		if (pBmpInfoHdr->biBitCount < 24)
		{
			TRACE("CameraObj::CaptureImage- WARNING: Images with bit depth less than 24bpp is not supported.\n");
			return RETURN_ERROR;
		}

	}

	CoTaskMemFree((LPVOID)pDIB);
	pVMR9ctrl->Release();
	return RETURN_SUCCESS;
}
