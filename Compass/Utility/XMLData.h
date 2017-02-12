#pragma once

#include <afx.h>

// this class holds all the Xml.config data
struct sXMLData
{
	// List of all the settings that should be saved in the XML.config file , expand as needed!
	
	// a Boolean that indicates if the file exists on the Disc
	BOOL m_bFromFile = FALSE; 
	// if data changed by user you need to update it.
	BOOL m_bNeedUpdate = FALSE; 

///////////////////XML DATA //////////////////////	
	
	CString m_strSlots=_T("");
	// The ID that differentiates between two cameras with the same FriendlyName(Very Important)
	CString m_strCurrentCamPath = _T(""); 
	// a Friendly name that can be used in UI controls (like a combo box).
	CString m_strCurrentCamFriendlyName = _T("");
};