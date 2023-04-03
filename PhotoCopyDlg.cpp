
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

void FormatDebugString(char* fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	char dbg_out[4096];
	vsprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugStringA(dbg_out);
}

CStringA PIDtoProcessName(DWORD pid) {
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	char name[256];
	DWORD len = 255;
	QueryFullProcessImageNameA(handle, 0, name, &len);
	CloseHandle(handle);
	char* exe = strrchr(name, '\\') + 1;

	return CStringA(exe);
}

// CPhotoCopyDlg message handlers

BOOL CPhotoCopyDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	bool status = RegisterHotKey(m_hWnd, MY_HOTKEY_ID, MOD_CONTROL, KEYS_RIGHT);
	FormatDebugString("Hotkey Register %d\n", status);

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
	if (nHotKeyId == MY_HOTKEY_ID) {
		SendHotKeys();
	}
}

void CPhotoCopyDlg::SendHotKeys() {
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
}

void CPhotoCopyDlg::LogCurrentWindow(HWND current) {
	char title[256];
	::GetWindowTextA(current, (LPSTR) &title, 255);

	DWORD pid = 0;
	GetWindowThreadProcessId(current, &pid);

	CStringA name = PIDtoProcessName(pid);

	FormatDebugString("Source: exe: %s, title: %s, pid: %d\n", (LPCSTR) name, title, pid);
}

static const CStringA EXPLORER = CStringA("explorer.exe");
BOOL CALLBACK EnumCallback(HWND hwnd, LPARAM lParam) {
	if (!::IsWindowVisible(hwnd) || ::GetWindowTextLength(hwnd) == 0) {
		return TRUE;
	}

	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);

	CStringA name = PIDtoProcessName(pid);
	if (name == EXPLORER) {
		((HWND*) lParam)[0] = hwnd;

		char title[256];
		::GetWindowTextA(hwnd, (LPSTR) &title, 255);
		FormatDebugString("Target: exe: %s, title: %s, pid: %d\n", (LPCSTR) name, title, pid);
		return FALSE;
	}

	return TRUE;
}

HWND CPhotoCopyDlg::FindExplorer() {
	HWND ret[1];
	::EnumDesktopWindows(NULL, EnumCallback, (LPARAM) ret);
	return ret[0];
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPhotoCopyDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

void CPhotoCopyDlg::PostNcDestroy() {
	CDialogEx::PostNcDestroy();
	delete this;
}

void CPhotoCopyDlg::OnOk() {
	DestroyWindow();
}

void CPhotoCopyDlg::OnCancel() {
	DestroyWindow();
}
