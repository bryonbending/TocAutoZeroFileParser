
// TocAutoZeroFileParser.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTocAutoZeroFileParserApp:
// See TocAutoZeroFileParser.cpp for the implementation of this class
//

class CTocAutoZeroFileParserApp : public CWinApp
{
public:
	CTocAutoZeroFileParserApp();

// Overrides
public:
	virtual BOOL InitInstance();

	CStringArray tcTempCorr;
	CStringArray icTempCorr;
	DOUBLE icMax1 = 0;
	DOUBLE tcMax1 = 0;
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTocAutoZeroFileParserApp theApp;
