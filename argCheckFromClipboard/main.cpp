#include <Windows.h>

#include <string>

#include "../../lsMisc/GetLastErrorString.h"
#include "../../lsMisc/GetClipboardText.h"
#include "../../lsMisc/OpenCommon.h"
#include "../../lsMisc/stdosd/stdosd.h"

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace std;

#define APPNAME L"argCheckFromClipboard"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	wstring text;
	if (!GetClipboardText(nullptr, text))
	{
		MessageBox(nullptr, GetLastErrorString(GetLastError()).c_str(), APPNAME, MB_ICONHAND);
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