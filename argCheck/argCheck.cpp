﻿//BSD 2-Clause License
//
//Copyright (c) 2017, Ambiesoft
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//* Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//* Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "stdafx.h"
#include <sstream>

#include "../../lsMisc/CommandLineString.h"
#include "../../lsMisc/HighDPI.h"
#include "../../lsMisc/OpenCommon.h"
#include "../../lsMisc/EnableTextTripleClickSelectAll.h"
#include "../../lsMisc/I18N.h"
#include "../../lsMisc/GetVersionString.h"

#include "argCheck.h"



using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

#define MAX_LOADSTRING 100
#define APPNAME L"argCheck"
#define KAIGYO L"\r\n"
#define HORIZLINE L"----------------------------------"

HINSTANCE ghInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

wstring gIni;

struct MainDialogData {
	wstring title_;
	wstring message_;
	wstring commnadline_;
	bool again_;
	bool bWW_;
};

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		wstring version = GetVersionString(stdGetModuleFileName().c_str(), 3);
		SetDlgItemText(hDlg, IDC_STATIC_TITLEANDVERSION,
			stdFormat(L"%s v%s", APPNAME, version.c_str()).c_str());
		return (INT_PTR)TRUE;
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON_GOTOWEB)
		{
			OpenCommon(hDlg, L"https://ambiesoft.github.io/webjumper/?target=argCheck");
		}
		break;
	}
	return (INT_PTR)FALSE;
}

wstring GetAllEnv()
{
	extern wchar_t** _wenviron; // _wenvironはwchar_tでの環境変数のリストを指す外部変数

	wstringstream wss;
	for (wchar_t** env = _wenviron; *env != nullptr; ++env) {
		std::wstring envString(*env); // 環境変数のwstringをコピー

		// KEY=VALUE形式に分割して表示
		size_t pos = envString.find(L'=');
		if (pos != std::wstring::npos) {
			std::wstring key = envString.substr(0, pos);
			std::wstring value = envString.substr(pos + 1);
			wss << key << L"=" << value << L"\r\n";
		}
	}

	return wss.str();
}


wstring ParseCommandLine(LPCWSTR pCommnadLine = nullptr)
{
	bool bUserInput = pCommnadLine != nullptr;
	if (!pCommnadLine)
	{
		pCommnadLine = GetCommandLine();
	}

	wstring message;

	message += I18N(L"Current Directory");
	message += L":";
	message += KAIGYO;
	message += stdGetCurrentDirectory();
	message += KAIGYO;
	message += KAIGYO;



	message += I18N(L"Command Line");
	message += L":";
	message += KAIGYO;
	message += pCommnadLine;
	message += KAIGYO;
	message += KAIGYO;


	message += I18N(L"Length");
	message += L":";
	message += KAIGYO;
	message += std::to_wstring(lstrlen(pCommnadLine));
	message += KAIGYO;
	message += KAIGYO;

	message += I18N(L"Environment");
	message += L":";
	message += KAIGYO;
	message += GetAllEnv();
	message += KAIGYO;
	message += KAIGYO;

	std::vector<wstring> crtResults;
	std::vector<wstring> commandLineToArgvWResults;
	std::vector<wstring> CCommandLineStringResults;

	// CRT
	if (bUserInput)
	{
		message += L"CRT is not available.";
		message += KAIGYO;
		message += HORIZLINE;
		message += KAIGYO;
		message += KAIGYO;
	}
	else
	{
		message += L"CRT";
		message += KAIGYO;
		message += HORIZLINE;
		message += KAIGYO;
		{
			message += I18N(L"CRT argc");
			message += L":";
			message += KAIGYO;
			message += to_wstring(__argc);
			message += KAIGYO;
			message += KAIGYO;

			for (int i = 0; i < __argc; ++i)
			{
				message += I18N(L"CRT Argument");
				message += L" ";
				message += to_wstring(i);
				message += L":";
				message += KAIGYO;
				message += __wargv[i];
				crtResults.push_back(__wargv[i]);
				message += KAIGYO;
				message += KAIGYO;
			}
		}
	}

	// CommandLineToArgvW
	message += L"CommandLineToArgvW";
	message += KAIGYO;
	message += HORIZLINE;
	message += KAIGYO;
	{
		int nNumArgs = 0;
		LPWSTR* pArgv = CommandLineToArgvW(pCommnadLine, &nNumArgs);
		message += I18N(L"Shell argc");
		message += L":";
		message += KAIGYO;
		message += to_wstring(nNumArgs);
		message += KAIGYO;
		message += KAIGYO;

		for (int i = 0; i < nNumArgs; ++i)
		{
			message += I18N(L"Shell Argument");
			message += L" ";
			message += to_wstring(i);
			message += L":";
			message += KAIGYO;
			message += pArgv[i];
			commandLineToArgvWResults.push_back(pArgv[i]);
			message += KAIGYO;
			message += KAIGYO;
		}
		LocalFree(pArgv);
	}

	// CCommandLineString
	message += L"CCommandLineString";
	message += KAIGYO;
	message += HORIZLINE;
	message += KAIGYO;
	{
		int nNumArgs = 0;
		LPWSTR* pArgv = CCommandLineString::getCommandLineArg(pCommnadLine, &nNumArgs);
		message += I18N(L"CCommandLineString argc");
		message += L":";
		message += KAIGYO;
		message += to_wstring(nNumArgs);
		message += KAIGYO;
		message += KAIGYO;

		for (int i = 0; i < nNumArgs; ++i)
		{
			message += I18N(L"CCommandLineString Argument");
			message += L" ";
			message += to_wstring(i);
			message += L":";
			message += KAIGYO;
			message += pArgv[i];
			CCommandLineStringResults.push_back(pArgv[i]);
			message += KAIGYO;
			message += KAIGYO;
		}
		CCommandLineString::freeCommandLineArg(pArgv);
	}

	wstring preMessage;
	if (stdIsAllEqual(crtResults, commandLineToArgvWResults, CCommandLineStringResults))
	{
		preMessage += I18N(L"The results of three methods of parsing Commandline are SAME");
	}
	else
	{
		preMessage += I18N(L"The results of three methods of parsing Commandline are NOT same");
	}
	
	preMessage += KAIGYO;
	preMessage += HORIZLINE;
	preMessage += KAIGYO;
	
	return preMessage + message;
}

//// http://rarara.cafe.coocan.jp/cgi-bin/lng/vc/vclng.cgi?print+200807/08070047.txt
//BOOL GetRightTurn(HWND hEdit)
//{
//	LONG lStyle = GetWindowLong(hEdit, GWL_STYLE);
//
//	return (lStyle & ES_AUTOHSCROLL) ? FALSE : TRUE;
//}
//BOOL SetRightTurn(HWND hEdit, BOOL bRightTurn)
//{
//	BOOL bRight = GetRightTurn(hEdit);
//	LONG lStyle = GetWindowLong(hEdit, GWL_STYLE);
//
//	if (bRightTurn){
//		lStyle &= ~(WS_HSCROLL | ES_AUTOHSCROLL);
//	}
//	else{
//		lStyle |= (WS_HSCROLL | ES_AUTOHSCROLL);
//	}
//	SetWindowLong(hEdit, GWL_STYLE, lStyle);
//	SetWindowPos(hEdit, NULL, 0, 0, 0, 0,
//		(SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED));
//	return bRight;
//}

void setWW(HWND hDlg, BOOL bOn)
{
	if (bOn)
	{
		// WW on
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT_MAINWW), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT_MAIN), SW_HIDE);
	}
	else
	{
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT_MAINWW), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT_MAIN), SW_SHOW);
	}
}

wstring GetDialogText(HWND hDlg, UINT id)
{
	int len = GetWindowTextLength(GetDlgItem(hDlg, id));
	std::vector<wchar_t> buff(len + 1);
	GetDlgItemText(hDlg, id, &buff[0], len + 1);
	buff[len] = 0;
	return &buff[0];
}


INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static MainDialogData* spData;
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		spData = (MainDialogData*)lParam;

		HWND i18nExcepts[] = {
			GetDlgItem(hDlg,IDOK),
		};
		i18nChangeDialogText(hDlg, i18nExcepts, _countof(i18nExcepts));

		i18nChangeMenuText(GetMenu(hDlg));


		SetWindowText(hDlg, spData->title_.c_str());
		
		SetDlgItemText(hDlg, IDC_EDIT_COMMANDLINE, spData->commnadline_.c_str());
		EnableTextTripleClickSelectAll(GetDlgItem(hDlg, IDC_EDIT_COMMANDLINE));


		SetDlgItemText(hDlg, IDC_EDIT_MAIN, spData->message_.c_str());
		SetDlgItemText(hDlg, IDC_EDIT_MAINWW, spData->message_.c_str());

		int intval = GetPrivateProfileInt(L"Option", L"WordWrap", 0, gIni.c_str()) !=0;
		setWW(hDlg,intval);
		SendDlgItemMessage(hDlg, IDC_CHECK_WORDWRAP, BM_SETCHECK, intval, 0);

		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ARGCHECK));
		SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

		CenterWindow(hDlg);
		return TRUE;
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			if(GetFocus()==GetDlgItem(hDlg, IDC_EDIT_COMMANDLINE))
			{ 
				PostMessage(hDlg, WM_COMMAND, IDC_BUTTON_REPARSE, 0);
				break;
			}
			// through
		case IDM_EXIT:
		{
			LRESULT ww = SendDlgItemMessage(hDlg, IDC_CHECK_WORDWRAP, BM_GETCHECK, 0, 0);
			if (!WritePrivateProfileString(L"Option", L"WordWrap", ww ? L"1" : L"0", gIni.c_str()))
			{
				MessageBox(hDlg, L"Save failed", APPNAME, MB_ICONERROR);
			}
			EndDialog(hDlg, IDOK);
			return 0;
		}
		break;

		case IDCANCEL:
		{
			EndDialog(hDlg, IDCANCEL);
			return 0;
		}
		break;

		case IDC_BUTTON_REPARSE:
		{
			wstring text = GetDialogText(hDlg, IDC_EDIT_COMMANDLINE);
			wstring message = ParseCommandLine(text.c_str());

			SetDlgItemText(hDlg, IDC_EDIT_MAIN, message.c_str());
			SetDlgItemText(hDlg, IDC_EDIT_MAINWW, message.c_str());
		}
		break;

		case IDC_BUTTON_NEWINSTANCE:
		{
			wstring exe = stdGetModuleFileName();
			wstring text = GetDialogText(hDlg, IDC_EDIT_COMMANDLINE);

			OpenCommon(hDlg,
				exe.c_str(),
				text.c_str());
		}
		break;

		case IDC_CHECK_WORDWRAP:
		{
			//SetRightTurn(GetDlgItem(hDlg, IDC_EDIT_MAIN),
			//	0 != SendDlgItemMessage(hDlg, IDC_CHECK_WORDWRAP, BM_GETCHECK, 0, 0));
			setWW(hDlg,0 != SendDlgItemMessage(hDlg, IDC_CHECK_WORDWRAP, BM_GETCHECK, 0, 0));
		}
		break;

		case IDM_ABOUT:
		{
			DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
		}
		break;

		
		}
	}
	break;
	}
	return FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	i18nInitLangmap(NULL, NULL, APPNAME);

	ghInst = hInstance;

	InitHighDPISupport();

	gIni = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName()), 
		stdGetFileNameWithoutExtension(APPNAME) + L".ini");

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ARGCHECK, szWindowClass, MAX_LOADSTRING);


	wstring message = ParseCommandLine();

	MainDialogData data;
	do
	{
		data.title_ = szTitle;
		data.message_ = message;
		data.commnadline_ = GetCommandLine();
		data.again_ = false;
		if (IDOK != DialogBoxParam(hInstance,
			MAKEINTRESOURCE(IDD_DIALOG_MAIN),
			NULL,
			MainDlgProc,
			(LPARAM)&data))
		{
			return 100;
		}
	} while (data.again_);

	return 0;
}


