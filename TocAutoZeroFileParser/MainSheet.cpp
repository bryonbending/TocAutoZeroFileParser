// CMainSheet.cpp : implementation file
//

#include "pch.h"
#include "TocAutoZeroFileParser.h"
#include "MainSheet.h"


// CMainSheet

IMPLEMENT_DYNAMIC(CMainSheet, CPropertySheet)

CMainSheet::CMainSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	generalPage = nullptr;
	AddPages();
}

CMainSheet::CMainSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	generalPage = nullptr;
	AddPages();
}

CMainSheet::~CMainSheet()
{
	RemovePages();
}


BEGIN_MESSAGE_MAP(CMainSheet, CPropertySheet)
END_MESSAGE_MAP()


// CMainSheet message handlers

void CMainSheet::AddPages()
{
	CString pageTitle(L"");

	RemovePages();

	if (generalPage == nullptr)
	{
		generalPage = new CGeneralPage();
		pageTitle = L"General";
		generalPage->m_psp.dwFlags |= PSP_USETITLE;
		generalPage->m_psp.pszTitle = pageTitle.AllocSysString();
	}
	AddPage(generalPage);

	if (graphPage == nullptr)
	{
		graphPage = new CGraphPage();
		pageTitle = L"Graph";
		graphPage->m_psp.dwFlags |= PSP_USETITLE;
		graphPage->m_psp.pszTitle = pageTitle.AllocSysString();
	}
	AddPage(graphPage);
}

void CMainSheet::RemovePages()
{
	// Remove the pages
	while (GetPageCount())
	{
		CPropertyPage* page = GetPage(GetPageCount() - 1);
		if (page != nullptr)
		{
			RemovePage(page);
			delete page;
		}
	}
}