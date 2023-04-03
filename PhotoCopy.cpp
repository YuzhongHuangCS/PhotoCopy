
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

CPhotoCopyApp::CPhotoCopyApp() {
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPhotoCopyApp object

CPhotoCopyApp theApp;


// CPhotoCopyApp initialization

BOOL CPhotoCopyApp::InitInstance() {
	CWinApp::InitInstance();
	m_pDialog = new CPhotoCopyDlg();
	m_pDialog->Create(IDD_PHOTOCOPY_DIALOG);
	m_pMainWnd = m_pDialog;

	return TRUE;
}

BOOL CPhotoCopyApp::ExitInstance() {
	CWinApp::ExitInstance();
	delete m_pDialog;

	return TRUE;
}
