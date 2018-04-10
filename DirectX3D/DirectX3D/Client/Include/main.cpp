#include "Client.h"
#include "Core/Core.h"

GP_USING

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!GET_SINGLE(CCore)->Init(hInstance, L"Direct3D", L"Direct3D", IDI_ICON1, 1280, 720, true))
	{
		DESTROY_SINGLE(CCore);
		return 0;
	}

	int iRet = GET_SINGLE(CCore)->Run();

	DESTROY_SINGLE(CCore);

	return iRet;
}