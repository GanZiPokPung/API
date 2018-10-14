
#include "Core.h"
#include "resource.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!CCore::GetInst()->Init(hInstance, L"GameFramework", L"GameFramework",
		IDI_ICON1, 1280, 720))
	{
		CCore::DestroyInst();
		return 0;
	}

	int iRet = CCore::GetInst()->Run();

	CCore::DestroyInst();

	return iRet;
}