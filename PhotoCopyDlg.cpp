
// PhotoCopyDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PhotoCopy.h"
#include "PhotoCopyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define KEYEVENTF_KEYDOWN 0x0000
#define KEYS_C 0x43
#define KEYS_V 0x56

// CPhotoCopyDlg dialog
CPhotoCopyDlg::CPhotoCopyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PHOTOCOPY_DIALOG, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhotoCopyDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPhotoCopyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_HOTKEY()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

void FormatDebugString(TCHAR* fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	TCHAR dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}

CString PIDtoProcessName(DWORD pid) {
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	TCHAR name[256];
	DWORD len = 255;
	QueryFullProcessImageName(handle, 0, name, &len);
	CloseHandle(handle);
	TCHAR* exe = _tcsrchr(name, '\\') + 1;

	return CString(exe);
}

// CPhotoCopyDlg message handlers

BOOL CPhotoCopyDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	bool status_right = RegisterHotKey(m_hWnd, VK_RIGHT, MOD_CONTROL, VK_RIGHT);
	bool status_left = RegisterHotKey(m_hWnd, VK_LEFT, MOD_CONTROL, VK_LEFT);
	// FormatDebugString(_T("Hotkey Register status: right=%d, left=%d\n"), status_right, status_left);

	CString str;
	str.Format(_T("Hotkey Register status: right=%d, left=%d\r\n"), status_right, status_left);
	LOG += str;
	SetDlgItemText(IDC_EDIT1, LOG);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPhotoCopyDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialogEx::OnPaint();
	}
}


void CPhotoCopyDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2) {
	SendHotKeys(nHotKeyId);
}

void CPhotoCopyDlg::SendHotKeys(UINT key) {
	keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYDOWN, 0);
	keybd_event(KEYS_C, 0, KEYEVENTF_KEYDOWN, 0);
	keybd_event(KEYS_C, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);

	HWND current = ::GetForegroundWindow();
	LogCurrentWindow(current);

	HWND explorer = FindExplorer();
	::SetForegroundWindow(explorer);
	Sleep(100);

	keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYDOWN, 0);
	keybd_event(KEYS_V, 0, KEYEVENTF_KEYDOWN, 0);
	keybd_event(KEYS_V, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
	Sleep(100);

	::SetForegroundWindow(current);
	Sleep(100);
	keybd_event(key, 0, KEYEVENTF_KEYDOWN, 0);
	keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

void CPhotoCopyDlg::LogCurrentWindow(HWND current) {
	TCHAR title[256];
	::GetWindowText(current, (LPTSTR) &title, 255);

	DWORD pid = 0;
	GetWindowThreadProcessId(current, &pid);

	CString name = PIDtoProcessName(pid);
	// FormatDebugString(_T("Source: exe: %s, title: %s, pid: %d\n"), (LPCTSTR) name, title, pid);

	CString str;
	str.Format(_T("Source: exe: %s, title: %s, pid: %d\r\n"), (LPCTSTR) name, title, pid);

	LOG += str;
	SetDlgItemText(IDC_EDIT1, LOG);
}

static const CString EXPLORER = CString(_T("explorer.exe"));
BOOL CALLBACK EnumCallback(HWND hwnd, LPARAM lParam) {
	if (!IsWindowVisible(hwnd) || GetWindowTextLength(hwnd) == 0) {
		return TRUE;
	}

	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);

	CString name = PIDtoProcessName(pid);
	if (name == EXPLORER) {
		TCHAR title[256];
		::GetWindowText(hwnd, (LPTSTR) &title, 255);
		// FormatDebugString(_T("Target: exe: %s, title: %s, pid: %d\n"), (LPCTSTR) name, title, pid);

		CString str;
		str.Format(_T("Target: exe: %s, title: %s, pid: %d\r\n"), (LPCTSTR) name, title, pid);

		((CString*) lParam)[0] += str;
		((HWND*) lParam)[1] = hwnd;
		return FALSE;
	}

	return TRUE;
}

HWND CPhotoCopyDlg::FindExplorer() {
	HWND ret[2];

	// Hack: sizeof(HWND) == sizeof(CString), [0] is CString, [1] is HWND
	memcpy(ret, &LOG, sizeof(CString));
	EnumDesktopWindows(NULL, EnumCallback, (LPARAM) ret);
	memcpy(&LOG, ret, sizeof(CString));
	SetDlgItemText(IDC_EDIT1, LOG);

	return ret[1];
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPhotoCopyDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

void CPhotoCopyDlg::OnCancel() {
	UnregisterHotKey(m_hWnd, VK_RIGHT);
	UnregisterHotKey(m_hWnd, VK_LEFT);
	DestroyWindow();
}
