// GeneralPage.cpp : implementation file
//

#include "pch.h"
#include "TocAutoZeroFileParser.h"
#include "afxdialogex.h"
#include "GeneralPage.h"


// CGeneralPage dialog

IMPLEMENT_DYNAMIC(CGeneralPage, CPropertyPage)

CGeneralPage::CGeneralPage()
	: CPropertyPage(IDD_GENERAL_PAGE)
{

}

CGeneralPage::~CGeneralPage()
{
}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILE, fileEditCtrl);
	DDX_Control(pDX, IDC_EDIT_MAX_TC, maxTcEditCtrl);
	DDX_Control(pDX, IDC_EDIT_MAX_IC, maxIcEditCtrl);
	DDX_Control(pDX, IDC_EDIT_MAX_TC2, maxTc2EditCtrl);
	DDX_Control(pDX, IDC_EDIT_MAX_IC2, maxIc2EditCtrl);
	DDX_Control(pDX, IDC_EDIT_AVG_TC, avgTcEditCtrl);
	DDX_Control(pDX, IDC_EDIT_AVG_IC, avgIcEditCtrl);
	DDX_Control(pDX, IDC_EDIT_RESULT_TC, resultTcEditCtrl);
	DDX_Control(pDX, IDC_EDIT_RESULT_IC, resultIcEditCtrl);
	DDX_Control(pDX, IDC_EDIT_FILE_NAME, fileNameEditCtrl);
}


BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PARSE_FILE, &CGeneralPage::OnBnClickedButtonParseFile)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CGeneralPage::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CGeneralPage::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CGeneralPage message handlers
void CGeneralPage::OnBnClickedButtonParseFile()
{
	const CString SaveCaptureBG("DC:Save Capture BG");
	const CString DcIc1("DC:IC1");
	const CString DcTc1("DC:TC1");
	const INT ZeroCaptureLength = 672;
	const CString CommaDelimiter(",");

	BOOL ret = false;
	CStdioFile* stdIoFile = new CStdioFile();
	CFileException* pFileException = nullptr;
	CString fileName("");
	CString errorMessage("");
	CString line("");
	CString fileContents("");
	CStringArray lines;
	CStringArray icLines;
	CStringArray tcLines;
	INT lineCount = 0;
	INT foundSaveBgLine = 0;
	INT startZeroCapture = 0;
	INT tokenPos = 0;
	INT tokenCount = 0;
	DOUBLE icMax2 = 0;
	DOUBLE tcMax2 = 0;
	DOUBLE icAvg = 0;
	DOUBLE tcAvg = 0;
	DOUBLE icResult = 0;
	DOUBLE tcResult = 0;

	try
	{
		fileNameEditCtrl.GetWindowTextW(fileName);

		ClearControls();

		ret = stdIoFile->Open((LPCTSTR)fileName, CFile::modeRead | CFile::typeText, pFileException);

		if (ret)
		{
			theApp.tcTempCorr.RemoveAll();
			theApp.icTempCorr.RemoveAll();
			theApp.icMax1 = 0;
			theApp.tcMax1 = 0;

			while (stdIoFile->ReadString(line) > 0)
			{
				lines.Add(line);
				lineCount++;
				fileContents.AppendFormat(L"%s\r\n", (LPCTSTR)line);

				if (line.Find(SaveCaptureBG) != -1)
				{
					foundSaveBgLine = lineCount;
					startZeroCapture = foundSaveBgLine - (ZeroCaptureLength * 2);

					CString startLineCapture = lines[startZeroCapture];

					for (int i = 0; i < ZeroCaptureLength; i++)
					{
						CString tempLine = lines[startZeroCapture + i];
						TRACE(L"%s\n", tempLine);

						if (tempLine.Find(DcTc1) != -1)
						{
							tcLines.Add(tempLine);
						}
						else if (tempLine.Find(DcIc1) != -1)
						{
							icLines.Add(tempLine);
						}
						else
						{
							//
						}
					}
				}
			}
			fileEditCtrl.SetWindowTextW(fileContents);

			GetTCondArray(&tcLines, theApp.tcTempCorr);

			GetTCondArray(&icLines, theApp.icTempCorr);

			GetValues(&theApp.icTempCorr, theApp.icMax1, icMax2, icAvg, icResult);
			CString maxIcStr1("");
			CString maxIcStr2("");
			CString avgIcStr("");
			CString resultIcStr("");
			maxIcStr1.Format(L"%f", theApp.icMax1);
			maxIcStr2.Format(L"%f", icMax2);
			avgIcStr.Format(L"%f", icAvg);
			resultIcStr.Format(L"%f", icResult);
			TRACE("icMax1:%s icMax2:%s icAvg:%s\n", maxIcStr1, maxIcStr2, avgIcStr, resultIcStr);
			maxIcEditCtrl.SetWindowTextW(maxIcStr1);
			maxIc2EditCtrl.SetWindowTextW(maxIcStr2);
			avgIcEditCtrl.SetWindowTextW(avgIcStr);
			resultIcEditCtrl.SetWindowTextW(resultIcStr);

			GetValues(&theApp.tcTempCorr, theApp.tcMax1, tcMax2, tcAvg, tcResult);
			CString maxTcStr1("");
			CString maxTcStr2("");
			CString avgTcStr("");
			CString resultTcStr("");
			maxTcStr1.Format(L"%f", theApp.tcMax1);
			maxTcStr2.Format(L"%f", tcMax2);
			avgTcStr.Format(L"%f", tcAvg);
			resultTcStr.Format(L"%f", tcResult);
			TRACE("tcMax1:%s tcMax2:%s tcAvg:%s tcResult:%s\n", maxTcStr1, maxTcStr2, avgTcStr, resultTcStr);
			maxTcEditCtrl.SetWindowTextW(maxTcStr1);
			maxTc2EditCtrl.SetWindowTextW(maxTcStr2);
			avgTcEditCtrl.SetWindowTextW(avgTcStr);
			resultTcEditCtrl.SetWindowTextW(resultTcStr);
		}
		else
		{
			errorMessage.Format(L"Failed to open %s", (LPCTSTR)fileName);
			AfxMessageBox(errorMessage, MB_OK | MB_ICONERROR);

		}
	}
	catch (CException* e)
	{
		WCHAR msg[1024];
		e->GetErrorMessage(msg, 1024);
		errorMessage.AppendFormat(L"Failed OnBnClickedButtonParseFile.  This is the error that was returned:\n\n Error  : %s", msg);
		ret = FALSE;
		e->Delete();
	}

	if (stdIoFile != nullptr)
	{
		if (stdIoFile->m_hFile != CFile::hFileNull)
		{
			stdIoFile->Close();
		}
		delete stdIoFile;
		stdIoFile = nullptr;
	}

}

BOOL CGeneralPage::GetTCondArray(CStringArray* cellArray, CStringArray& tCondArray)
{
	const CString CommaDelimiter(",");

	BOOL ret = FALSE;

	if (cellArray != nullptr)
	{
		for (int i = 0; i < cellArray->GetCount(); i++)
		{
			CString line = cellArray->GetAt(i);
			TRACE(L"%s\n", line);

			INT tokenPos = 0;
			INT tokenCount = 0;
			CString token = line.Tokenize(CommaDelimiter, tokenPos);

			while (!token.IsEmpty() && ++tokenCount <= 5)
			{
				TRACE(L"%s\n", token);
				token = line.Tokenize(CommaDelimiter, tokenPos);
				if (tokenCount == 5)
				{
					CString tempToken("");
					tempToken.Format(L"%s", (LPCTSTR)token);
					tCondArray.Add(tempToken);
				}
			}
		}

		ret = TRUE;
	}

	return ret;
}

BOOL CGeneralPage::GetValues(CStringArray* stringArray, DOUBLE& maxValue1, DOUBLE& maxValue2, DOUBLE& avgValue, DOUBLE& resultValue)
{
	BOOL ret = FALSE;
	DOUBLE sum = 0;
	INT cnt = 0;
	maxValue1 = 0;
	maxValue2 = 0;
	avgValue = 0;
	resultValue = 0;

	if (stringArray != nullptr)
	{
		if (stringArray->GetCount() >= 300)
		{
			for (int i = 0; i < stringArray->GetCount(); i++)
			{
				DOUBLE d = _wtof(stringArray->GetAt(i));

				if (i < 50)
				{
					maxValue1 = d > maxValue1 ? d : maxValue1;
				}
				else if (i >= 54 && i < 250)
				{
					maxValue2 = d > maxValue2 ? d : maxValue2;
				}
				else if (i >= 275 && i < 290)
				{
					sum += d;
					cnt++;
				}
			}

			if (cnt > 0)
			{
				avgValue = sum / cnt;
			}

			resultValue = maxValue2 - avgValue;

			ret = TRUE;
		}
	}

	return ret;
}

void CGeneralPage::OnBnClickedButtonClear()
{
	ClearControls();
}

void CGeneralPage::ClearControls()
{
	fileEditCtrl.SetWindowTextW(L"");
	maxIcEditCtrl.SetWindowTextW(L"");
	maxTcEditCtrl.SetWindowTextW(L"");
	maxTc2EditCtrl.SetWindowTextW(L"");
	maxIc2EditCtrl.SetWindowTextW(L"");
	avgTcEditCtrl.SetWindowTextW(L"");
	avgIcEditCtrl.SetWindowTextW(L"");
	resultTcEditCtrl.SetWindowTextW(L"");
	resultIcEditCtrl.SetWindowTextW(L"");
}

void CGeneralPage::OnBnClickedButtonBrowse()
{
	CFileDialog fileDialog(TRUE, L"log", NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		L"Log Files (*.log)|*.log", this);

	fileDialog.m_pOFN->lpstrTitle = L"Open SL1 Log File";

	fileDialog.m_pOFN->lpstrInitialDir = L"C:\\tmp";

	if (fileDialog.DoModal() == IDOK)
	{
		TRACE(fileDialog.GetPathName());
		fileNameEditCtrl.SetWindowTextW(fileDialog.GetPathName());
		ClearControls();
	}
}


BOOL CGeneralPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	fileNameEditCtrl.SetWindowTextW(L"C:\\tmp\\24128943-teraterm-2024-12-18-Bay2.log");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
