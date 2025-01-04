#pragma once
#include "afxdialogex.h"


// CGeneralPage dialog

class CGeneralPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralPage)

public:
	CGeneralPage();
	virtual ~CGeneralPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GENERAL_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonParseFile();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonBrowse();
	DECLARE_MESSAGE_MAP()

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

	BOOL GetValues(CStringArray* stringArray, DOUBLE& maxValue1, DOUBLE& maxValue2, DOUBLE& avgValue, DOUBLE& resultValue);
	BOOL GetTCondArray(CStringArray* cellArray, CStringArray& tCondArray);
	void ClearControls();

	
};
