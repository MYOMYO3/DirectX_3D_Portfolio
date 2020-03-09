
//#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
//// Windows ��� ����
//#include <windows.h>

#include <JWINDOW.h>
#include <GRAPHICDEVICE.h>
#include <INPUT.h>
#include <MATHS.h>
#include <SCENE.h>

#include <DEBUG.h>
#include <GAMEWIN.h>

#include "InitFunc.h"
#include "LobySceneUpdater.h"




// C ��Ÿ�� ��� �����Դϴ�.w
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow){
	//new int;

//	Debug::OpenConsole();
	Debug::LeakCheck();
		
	// �����丵
	GameWindow::Init(hInstance);
	GameWindow::CreateGameWin(L"MainWindow");
	GameWindow::MainGameWin()->Pos(0, 0);
	GameWindow::MainGameWin()->Size(WIN_SIZE_X, WIN_SIZE_Y);
	GameWindow::MainGameWin()->CreateDevice();
	// ������ 
	ClientInit();
	GameWindow::EngineLoop();


	// HSCENE::EngineLoop();

	// ������ ���°� �޾���� �Ѵ�.
	 

	//Init();
	//HWINDOW::BasicPeekLoop(TestLoop);

	// HPTR<HSCENE> p = HSCENE::MainScene();
}
