#include "Core.h"
#include "Scene\SceneManager.h"
#include "Core\TimerManager.h"
#include "Core\Timer.h"
#include "Object\ObjectManager.h"
#include "Core\PathManager.h"
#include "Resources\ResourcesManager.h"
#include "Resources\Texture.h"
#include "Core\Input.h"
#include "Core\Camera.h"
#include "Collider\CollisionManager.h"

CCore* CCore::m_pInst = NULL;
bool CCore::m_bLoop = true;

CCore::CCore()	:
	m_pBackBuffer(NULL)
{
	// 디버깅 모드
	// 메모리 누수가 있는지 검사
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(100);

#ifdef _DEBUG
	// API 환경에서 콘솔을 띄운다.
	AllocConsole();
#endif
}


CCore::~CCore()
{
	// 생성헀던 모든 SINGLETON을 해제한다.
	// hWnd, hDC, backBuffer
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CCamera);
	DESTROY_SINGLE(CInput);
	SAFE_RELEASE(m_pBackBuffer);
	DESTROY_SINGLE(CResourcesManager);
	DESTROY_SINGLE(CPathManager);
	DESTROY_SINGLE(CObjectManager);
	DESTROY_SINGLE(CTimerManager);
	DESTROY_SINGLE(CSceneManager);
	ReleaseDC(m_hWnd, m_hDC);

#ifdef _DEBUG
	// API 환경에서 띄운 콘솔을 해제한다.
	FreeConsole();
#endif // _DEBUG
}

bool CCore::Init(HINSTANCE hInst, TCHAR * pTitle, TCHAR * pClass, int iIconID,
	UINT iWidth, UINT iHeight)
{
	m_hInst = hInst;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	
	// 앞으로 이러한 윈도우 클래스를 쓴다고 운영체제에게 알린다.
	WindowRegisterClass(pClass, iIconID);

	// 윈도우 창을 만들고 띄운다.(보여줌)
	InitWindow(pClass, pTitle, iWidth, iHeight);

	// GetDC 함수 앞에 :: 붙인 이유는 멤버함수 GetDC와 Win32API에서 제공하는 GetDC함수중
	// Win32에서 제공하는 함수를 호출하기 위해 앞에 ::을 붙여주었다.
	m_hDC = ::GetDC(m_hWnd);

	// 카메라 초기화
	if (!GET_SINGLE(CCamera)->Init(0.f, 0.f, (float)iWidth, (float)iHeight))
		return false;

	// 경로 관리자 초기화
	if (!GET_SINGLE(CPathManager)->Init())
		return false;

	// 타이머 관리자를 초기화.
	if (!GET_SINGLE(CTimerManager)->Init())
		return false;

	// 리소스 관리자를 초기화.
	if (!GET_SINGLE(CResourcesManager)->Init(m_hInst, m_hDC))
		return false;

	// 입력관리자를 초기화.
	if (!GET_SINGLE(CInput)->Init(m_hWnd))
		return false;

	// 충돌 관리자를 초기화.
	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

	// 오브젝트 관리자를 초기화.
	if (!GET_SINGLE(CObjectManager)->Init())
		return false;

	// 장면관리자를 초기화.
	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	// 백버퍼를 얻어온다.
	// 화면깜빡임 현상을 막기 위함(더블 버퍼링)
	m_pBackBuffer = GET_SINGLE(CResourcesManager)->FindTexture("BackBuffer");

	return true;
}

int CCore::Run()
{
	MSG msg;

	while (m_bLoop)
	{
		// 사용자 입력의 모든 메세지를 여기서 처리한다. 윈도우
		// 이게 없으면 키보드 마우스 등등의 입력 처리가 불가능하다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 윈도우 데드타임일 때는 else  구문으로 들어온다.
		else
		{
			//Game Logic을 수행한다.
			Logic();
		}
	}

	// PARAM은 파라미터의 줄인말이고 W는 WORD L은 LONG의 줄인말이다.
	return (int)msg.wParam;
}

void CCore::Logic()
{
	// 메인 타이머를 가져온다.
	CTimer*	pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainTimer");

	// 타이머 업데이트와 동시에
	pTimer->Update();

	// 타이머의 델타값을 기준으로 
    // Input, Update, Collision, Render를 수행한다.
	Input(pTimer->GetDeltaTime());
	Update(pTimer->GetDeltaTime());
	Collision(pTimer->GetDeltaTime());
	Render(pTimer->GetDeltaTime());
}

void CCore::Input(float fTime)
{
	// 입력 관리자 업데이트
	GET_SINGLE(CInput)->Update(fTime);

	// 엔터키 입력시 자유 카메라 시점
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		GET_SINGLE(CCamera)->SetTarget(NULL);

	// 카메라 조절
	GET_SINGLE(CCamera)->Input(fTime);
	// 장면 관리
	GET_SINGLE(CSceneManager)->Input(fTime);
}

void CCore::Update(float fTime)
{
	// 카메라와 장면을 업데이트 한다. time값 기준으로
	GET_SINGLE(CCamera)->Update(fTime);
	GET_SINGLE(CSceneManager)->Update(fTime);
}

void CCore::Collision(float fTime)
{
	// 마우스를 충돌 관리자에 등록한다.
	GET_SINGLE(CCollisionManager)->AddObject(GET_SINGLE(CInput)->GetMouse());
	GET_SINGLE(CCollisionManager)->Collision(fTime);
}

void CCore::Render(float fTime)
{
	// 장면 안에 있는 모든 오브젝트를 백버퍼에 그린다.
	GET_SINGLE(CSceneManager)->Render(m_pBackBuffer->GetMemDC(), fTime);

	//드래그 창 설정
	/*
	static POINT	ptStart, ptEnd;
	static bool		bStart;

	if (KEYDOWN("MouseLButton"))
	{
		// 마우스 왼쪽 클릭시 클릭 지점으로 부터 드래그 창이 시작된다.
		GetCursorPos(&ptStart);
		ScreenToClient(m_hWnd, &ptStart);
		bStart = true;
	}

	else if (KEYUP("MouseLButton"))
	{
		GetCursorPos(&ptEnd);
		ScreenToClient(m_hWnd, &ptEnd);
	}

	if (bStart)
	{
		// 드래그 시작시 마우스 위치에 따른 사각형을 그린다.
		HDC	hBackDC = m_pBackBuffer->GetMemDC();
		MoveToEx(hBackDC, ptStart.x, ptStart.y, NULL);
		POINT	ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(m_hWnd, &ptCursor);

		LineTo(hBackDC, ptCursor.x, ptStart.y);
		LineTo(hBackDC, ptCursor.x, ptCursor.y);
		LineTo(hBackDC, ptStart.x, ptCursor.y);
		LineTo(hBackDC, ptStart.x, ptStart.y);
	}
	*/

	// 마지막으로 마우스를 백버퍼에 그린다.
	GET_SINGLE(CInput)->GetMouse()->Render(m_pBackBuffer->GetMemDC(), fTime);

	// 마지막으로 백버퍼를 화면에 그린다.
	BitBlt(m_hDC, 0, 0, m_iWidth, m_iHeight, m_pBackBuffer->GetMemDC(),
		0, 0, SRCCOPY);
}

ATOM CCore::WindowRegisterClass(TCHAR * pClass, int iIconID)
{
	// WNDCLASS의 확장판
	// RegisterClassEx 사용을 위한 Window class 정보 구조체를 채운다.
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CCore::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(iIconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MY170825);
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iIconID));

	// 앞으로 위와 같은 특성을 가진 윈도우를 사용하겠다고 등록하는 절차이다. 
	// 운영체제는 이 윈도우 클래스의 특성을 잘 기억해 놓는다
	return RegisterClassExW(&wcex);
}

BOOL CCore::InitWindow(TCHAR* pClass, TCHAR * pTitle, UINT iWidth, UINT iHeight)
{
	// 윈도우 핸들에 윈도우를 만들어 대입시킨다.
	HWND hWnd = CreateWindowW(pClass, pTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, iWidth, iHeight, nullptr, nullptr, m_hInst, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	// 윈도우의 클라이언트 영역을 원하는 크기로 설정하는 방법
	RECT	rc = { 0, 0, iWidth, iHeight };

	// 크기와 위치를 조정한다.
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	// 멤버 데이터에 기존 핸들값을 대입
	m_hWnd = hWnd;

	// 창을 띄운다.(보여준다)
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// WndProc는 메시지가 발생할 때 프로그램의 반응을 처리하는 일을 한다.
	
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:			// 키를 누를때 들어오는 메세지이다.
		switch (wParam)
		{
		case VK_ESCAPE:			// ESC를 누르면 Window가 종료된다.
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
