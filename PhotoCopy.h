
// PhotoCopy.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPhotoCopyApp:
// See PhotoCopy.cpp for the implementation of this class
//

class CPhotoCopyApp : public CWinApp
{
public:
	CPhotoCopyApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
private:
	CDialogEx* m_pDialog;
};

extern CPhotoCopyApp theApp;
