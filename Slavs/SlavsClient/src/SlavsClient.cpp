#include <windows.h>
#include "Application.h"

template<> std::shared_ptr<Application> Singleton<Application>::mp_singleton = nullptr;

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
  new Singleton<Application>(new Application());
	try
	{
		Singleton<Application>::GetInstance().Start();
	}
	catch(std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    Singleton<Application>::ReleaseIfValid();
	}
  Singleton<Application>::ReleaseIfValid();
	return 0;
}