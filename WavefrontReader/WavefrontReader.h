// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// WavefrontReader.h : main header file for the WavefrontReader application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWavefrontReaderApp:
// See WavefrontReader.cpp for the implementation of this class
//

class CWavefrontReaderApp : public CWinAppEx
{
public:
	CWavefrontReaderApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	virtual void AddToRecentFileList(LPCTSTR filename);
	afx_msg void OnAppAbout();
	virtual void OnFileNew();
	DECLARE_MESSAGE_MAP()
};

extern CWavefrontReaderApp theApp;
