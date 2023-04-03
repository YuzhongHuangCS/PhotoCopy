
// PhotoCopyDlg.h : header file
//

#pragma once


// CPhotoCopyDlg dialog
class CPhotoCopyDlg : public CDialogEx
{
// Construction
public:
	CPhotoCopyDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHOTOCOPY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg HCURSOR OnQueryDragIcon();
	void OnOk();
	void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	const UINT MY_HOTKEY_ID = 1;
	const UINT KEYEVENTF_KEYDOWN = 0x0000;
	const UINT KEYS_C = 0x43;
	const UINT KEYS_V = 0x56;

	void SendHotKeys();
	void LogCurrentWindow(HWND current);
	HWND FindExplorer();

	CString LOG;
};
