
// TocAutoZeroFileParserDlg.h : header file
//

#pragma once


// CTocAutoZeroFileParserDlg dialog
class CTocAutoZeroFileParserDlg : public CDialogEx
{
// Construction
public:
	CTocAutoZeroFileParserDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOCAUTOZEROFILEPARSER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CEdit fileEditCtrl;
	CEdit maxTcEditCtrl;
	CEdit maxIcEditCtrl;
	CEdit maxTc2EditCtrl;
	CEdit maxIc2EditCtrl;
	CEdit avgTcEditCtrl;
	CEdit avgIcEditCtrl;
	CEdit resultTcEditCtrl;
	CEdit resultIcEditCtrl;
	CEdit fileNameEditCtrl;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonParseFile();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonBrowse();
	DECLARE_MESSAGE_MAP()

	BOOL GetValues(CStringArray *stringArray, DOUBLE &maxValue1, DOUBLE &maxValue2, DOUBLE &avgValue, DOUBLE &resultValue);
	BOOL GetTCondArray(CStringArray *cellArray, CStringArray& tCondArray);
	void CTocAutoZeroFileParserDlg::ClearControls();
	
};
