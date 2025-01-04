#pragma once
#include "afxdialogex.h"


// CGraphPage dialog

class CGraphPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGraphPage)

public:
	CGraphPage();
	virtual ~CGraphPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnSetActive();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	CStatic staticGraph;

	void DrawGraph();
	
	
};
