// GraphPage.cpp : implementation file
//

#include "pch.h"
#include "TocAutoZeroFileParser.h"
#include "afxdialogex.h"
#include "GraphPage.h"


// CGraphPage dialog

IMPLEMENT_DYNAMIC(CGraphPage, CPropertyPage)

CGraphPage::CGraphPage()
	: CPropertyPage(IDD_GRAPH_PAGE)
{

}

CGraphPage::~CGraphPage()
{
}

void CGraphPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphPage, CPropertyPage)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGraphPage message handlers


BOOL CGraphPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	DrawGraph();

	return CPropertyPage::OnSetActive();
}

void CGraphPage::DrawGraph()
{
	const DWORD PADDING = 4;
	const DWORD XAXIS_TEXT_OFFSET = 6;
	const DWORD YAXIS_TEXT_OFFSET = 9;
	const DWORD AXIS_AREA = 30;

	RECT rect = { 0 };
	RECT graph_rect = { 0 };
	RECT plot_area_rect = { 0 };
	POINT* pntTcArray = nullptr;
	POINT* pntIcArray = nullptr;
	CPen bluePen(PS_SOLID, 0, RGB(63, 72, 204));
	CPen orangePen(PS_SOLID, 0, RGB(255, 127, 39));
	CPen blackPen(PS_SOLID, 0, RGB(0, 0, 0));
	CPen lightGrayPen(PS_SOLID, 0, RGB(192, 192, 192));
	CBrush whiteBrush(RGB(255, 255, 255));
	CBrush blueBrush(RGB(63, 72, 204));
	CBrush orangeBrush(RGB(255, 127, 39));

	// Retrieve the handle to the display device context.
	//CPaintDC dc(this);
	CDC* pCDC = GetDC();
	HDC hDC = pCDC->GetSafeHdc();

	// Retrieves the coordinates of a window's client area. 
	GetClientRect(&rect);

	// Set the Graph Rect
	graph_rect.top = rect.top + PADDING;
	graph_rect.left = PADDING;
	graph_rect.right = rect.right - PADDING;
	graph_rect.bottom = rect.bottom - PADDING;

	plot_area_rect.top = graph_rect.top;
	plot_area_rect.left = graph_rect.left + AXIS_AREA;
	plot_area_rect.right = graph_rect.right;
	plot_area_rect.bottom = graph_rect.bottom - AXIS_AREA;

	// Graph Background
	pCDC->FillRect(&graph_rect, &whiteBrush);

	if (hDC == INVALID_HANDLE_VALUE)
		return;

	if (theApp.tcTempCorr.GetCount() > 0 && theApp.icTempCorr.GetCount() > 0)
	{
		// TC Point array
		pntTcArray = new POINT[theApp.tcTempCorr.GetCount()];
		memset(pntTcArray, 0, theApp.tcTempCorr.GetCount() * sizeof(*pntTcArray));
		// IC Point array
		pntIcArray = new POINT[theApp.icTempCorr.GetCount()];
		memset(pntIcArray, 0, theApp.icTempCorr.GetCount() * sizeof(*pntTcArray));

		float fDataWidth = (float)theApp.tcTempCorr.GetCount();
		float fDataHeight = theApp.icMax1 > theApp.tcMax1 ? (FLOAT)theApp.icMax1 : (FLOAT)theApp.tcMax1;
		fDataHeight += fDataHeight * 0.20;

		float fWRatio = (float)(plot_area_rect.right - plot_area_rect.left) / fDataWidth;
		float fHRatio = (float)(plot_area_rect.bottom - plot_area_rect.top) / fDataHeight;
		
		// Y Axis
		CPen* pOldPen = pCDC->SelectObject(&lightGrayPen);
		POINT pntYAxis = { 0 };

		DOUBLE y = 0;
		CString yAxisStr(L"");

		while (y < fDataHeight)
		{
			if (y > 0)
			{
				pCDC->SelectObject(&lightGrayPen);
			}
			else
			{
				pCDC->SelectObject(&blackPen);
			}
			pntYAxis.x = plot_area_rect.left;
			pntYAxis.y = (long)(plot_area_rect.bottom - (y * fHRatio));
			pCDC->MoveTo(pntYAxis);

			pntYAxis.x = plot_area_rect.right;
			pCDC->LineTo(pntYAxis);

			pCDC->SelectObject(&blackPen);
			pntYAxis.x = graph_rect.left;
			pntYAxis.y = (long)(plot_area_rect.bottom - YAXIS_TEXT_OFFSET - (y * fHRatio));
			yAxisStr.Format(L"%.1f", y);
			pCDC->TextOutW(pntYAxis.x, pntYAxis.y, yAxisStr);

			y += 0.1;
		}
		
		// Draw Y-Axis line
		pCDC->SelectObject(&blackPen);

		pntYAxis.x = plot_area_rect.left;
		pntYAxis.y = plot_area_rect.top;
		pCDC->MoveTo(pntYAxis);

		pntYAxis.y = plot_area_rect.bottom;
		pCDC->LineTo(pntYAxis);

		// X Axis
		DOUBLE x = 0;
		CString xAxisStr(L"");

		while (x < fDataWidth)
		{
			if (x > 0)
			{
				pCDC->SelectObject(&lightGrayPen);
				pntYAxis.x = (long)(plot_area_rect.left + (x * fWRatio));
				pntYAxis.y = (long)plot_area_rect.top;
				pCDC->MoveTo(pntYAxis);

				pntYAxis.y = plot_area_rect.bottom;
				pCDC->LineTo(pntYAxis);
			}

			pCDC->SelectObject(&blackPen);
			DWORD textOffset = 0;
			if (x > 0)
			{
				textOffset = XAXIS_TEXT_OFFSET * 2;
			}
			pntYAxis.x = (long)(plot_area_rect.left - textOffset + (x * fWRatio));
			pntYAxis.y = (long)(plot_area_rect.bottom + XAXIS_TEXT_OFFSET);
			xAxisStr.Format(L"%d", (INT)x);
			pCDC->TextOutW(pntYAxis.x, pntYAxis.y, xAxisStr);

			x += 50;
		}

		for (DWORD i = 0; i < theApp.tcTempCorr.GetCount(); i++)
		{
			pntTcArray[i].x = (long)(plot_area_rect.left + (i * fWRatio));
			pntIcArray[i].x = pntTcArray[i].x;

			DOUBLE tcValue = _wtof(theApp.tcTempCorr.GetAt(i));
			pntTcArray[i].y = (long)(plot_area_rect.bottom - (tcValue * fHRatio));
			TRACE(TEXT("TC:%d,%d"), pntTcArray[i].x, pntTcArray[i].y);

			DOUBLE icValue = _wtof(theApp.icTempCorr.GetAt(i));
			pntIcArray[i].y = (long)(plot_area_rect.bottom - (icValue * fHRatio));
			TRACE(TEXT("IC:%d,%d"), pntIcArray[i].x, pntIcArray[i].y);
			
		}

		// Series
		CRect seriesRect;
		pCDC->SelectObject(&lightGrayPen);
		seriesRect.left = plot_area_rect.right - 50 - PADDING ;
		seriesRect.top = plot_area_rect.top + PADDING;
		seriesRect.right = seriesRect.left + 50;
		seriesRect.bottom = seriesRect.bottom + 50;
		pCDC->FillRect(&seriesRect, &whiteBrush);
		pCDC->Rectangle(seriesRect);
		
		CString tcSeriesStr(L"TC");
		pntYAxis.x = (long)(plot_area_rect.right - 50);
		pntYAxis.y = (long)(plot_area_rect.top + 5);
		pCDC->TextOutW(pntYAxis.x, pntYAxis.y, tcSeriesStr);

		SIZE size = { 10, 10 };
		pntYAxis.x = seriesRect.right - 20 + PADDING;
		pntYAxis.y = seriesRect.top + 5;
		CRect seriesColor(pntYAxis, size);
		pCDC->FillRect(seriesColor, &blueBrush);

		pCDC->SelectObject(&bluePen);
		Polyline(hDC, pntTcArray, (INT)theApp.tcTempCorr.GetCount());
		
		CString icSeriesStr(L"IC");
		pntYAxis.x = (long)(plot_area_rect.right - 50);
		pntYAxis.y = (long)(plot_area_rect.top + 25);
		pCDC->TextOutW(pntYAxis.x, pntYAxis.y, icSeriesStr);

		pntYAxis.x = seriesRect.right - 20 + PADDING;
		pntYAxis.y = seriesRect.top + 25;
		seriesColor.left = pntYAxis.x;
		seriesColor.top = pntYAxis.y;
		seriesColor.right = seriesColor.left + size.cx;
		seriesColor.bottom = seriesColor.top + size.cy;
		pCDC->FillRect(seriesColor, &orangeBrush);

		pCDC->SelectObject(&orangePen);
		Polyline(hDC, pntIcArray, (INT)theApp.icTempCorr.GetCount());

		if (pOldPen != nullptr)
		{
			SelectObject(hDC, pOldPen);
			DeleteObject(pOldPen);
		}
	}

	if (pntTcArray != nullptr)
	{
		delete[] pntTcArray;
	}

	if (pntIcArray != nullptr)
	{
		delete[] pntIcArray;
	}

	ReleaseDC(pCDC);
}




void CGraphPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CPropertyPage::OnPaint() for painting messages
	DrawGraph();
}
