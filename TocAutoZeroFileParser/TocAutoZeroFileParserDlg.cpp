
// TocAutoZeroFileParserDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TocAutoZeroFileParser.h"
#include "TocAutoZeroFileParserDlg.h"
#include "afxdialogex.h"
#

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTocAutoZeroFileParserDlg dialog



CTocAutoZeroFileParserDlg::CTocAutoZeroFileParserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TOCAUTOZEROFILEPARSER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTocAutoZeroFileParserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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

BEGIN_MESSAGE_MAP(CTocAutoZeroFileParserDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PARSE_FILE, &CTocAutoZeroFileParserDlg::OnBnClickedButtonParseFile)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CTocAutoZeroFileParserDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CTocAutoZeroFileParserDlg::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CTocAutoZeroFileParserDlg message handlers

BOOL CTocAutoZeroFileParserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	fileNameEditCtrl.SetWindowTextW(L"C:\\tmp\\24128943-teraterm-2024-12-18-Bay2.log");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTocAutoZeroFileParserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTocAutoZeroFileParserDlg::OnPaint()
{
	if (IsIconic())
	{
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
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTocAutoZeroFileParserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTocAutoZeroFileParserDlg::OnBnClickedButtonParseFile()
{
	const CString SaveCaptureBG("DC:Save Capture BG");
	const CString DcIc1("DC:IC1");
	const CString DcTc1("DC:TC1");
	const INT ZeroCaptureLength = 672;
	const CString CommaDelimiter(",");

	BOOL ret = false;
	CStdioFile *stdIoFile = new CStdioFile();
	CFileException* pFileException = nullptr;
	CString fileName("");
	CString errorMessage("");
	CString line("");
	CString fileContents("");
	CStringArray lines;
	CStringArray icLines;
	CStringArray tcLines;
	CStringArray tcTempCorr;
	CStringArray icTempCorr;
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

			GetTCondArray(&tcLines, tcTempCorr);

			GetTCondArray(&icLines, icTempCorr);

			GetValues(&icTempCorr, theApp.icMax1, icMax2, icAvg, icResult);
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

			GetValues(&tcTempCorr, theApp.tcMax1, tcMax2, tcAvg, tcResult);
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
	catch (CException *e)
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

BOOL CTocAutoZeroFileParserDlg::GetTCondArray(CStringArray* cellArray, CStringArray &tCondArray)
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

BOOL CTocAutoZeroFileParserDlg::GetValues(CStringArray *stringArray, DOUBLE &maxValue1, DOUBLE &maxValue2, DOUBLE &avgValue, DOUBLE &resultValue)
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

void CTocAutoZeroFileParserDlg::OnBnClickedButtonClear()
{
	ClearControls();
}

void CTocAutoZeroFileParserDlg::ClearControls()
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

void CTocAutoZeroFileParserDlg::OnBnClickedButtonBrowse()
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
