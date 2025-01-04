#pragma once
#include "GeneralPage.h"
#include "GraphPage.h"


// CMainSheet

class CMainSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMainSheet)

public:
	CMainSheet(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	CMainSheet(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	virtual ~CMainSheet();

	void AddPages();
	void RemovePages();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CGeneralPage* generalPage;
	CGraphPage* graphPage;
};


