#include <Windows.h>

#include <string>

#include "../../lsMisc/GetLastErrorString.h"
#include "../../lsMisc/GetClipboardText.h"
#include "../../lsMisc/OpenCommon.h"
#include "../../lsMisc/stdosd/stdosd.h"
#include "../../lsMisc/I18N.h"

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace std;

#define APPNAME L"argCheckFromClipboard"

#define KAIGYO L"\r\n";

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	i18nInitLangmap(NULL, NULL, APPNAME);
	wstring text;
	if (!GetClipboardText(nullptr, text))
	{
		DWORD dwLE = GetLastError();
		wstring message;
		message += I18N(L"Failed to get text from clipboard.");
		if (dwLE != NO_ERROR)
		{
			message += KAIGYO;
			message += KAIGYO;
			message += GetLastErrorString(dwLE);
		}
		MessageBox(nullptr, message.c_str(), APPNAME, MB_ICONHAND);
		return 1;
	}

	wstring app = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()),
		L"argCheck.exe");
	if (!OpenCommon(nullptr, app.c_str(), text.c_str()))
	{
		MessageBox(nullptr, GetLastErrorString(GetLastError()).c_str(), APPNAME, MB_ICONHAND);
		return 1;
	}
	return 0;
}