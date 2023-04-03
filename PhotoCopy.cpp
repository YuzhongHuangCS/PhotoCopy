
// PhotoCopy.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "PhotoCopy.h"
#include "PhotoCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPhotoCopyApp

BEGIN_MESSAGE_MAP(CPhotoCopyApp, CWinApp)
END_MESSAGE_MAP()


// CPhotoCopyApp construction

CPhotoCopyApp::CPhotoCopyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPhotoCopyApp object

CPhotoCopyApp theApp;


// CPhotoCopyApp initialization

BOOL CPhotoCopyApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	m_pDialog = new CPhotoCopyDlg();
	m_pMainWnd = m_pDialog;

	m_pDialog->Create(IDD_PHOTOCOPY_DIALOG);

	return TRUE;
}

