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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(100);

#ifdef _DEBUG
	AllocConsole();
#endif
}


CCore::~CCore()
{
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
	FreeConsole();
#endif // _DEBUG
}

bool CCore::Init(HINSTANCE hInst, TCHAR * pTitle, TCHAR * pClass, int iIconID,
	UINT iWidth, UINT iHeight)
{
	m_hInst = hInst;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	
	WindowRegisterClass(pClass, iIconID);

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

	// 타이머 관리자를 초기화한다.
	if (!GET_SINGLE(CTimerManager)->Init())
		return false;

	// 리소스 관리자를 초기화한다.
	if (!GET_SINGLE(CResourcesManager)->Init(m_hInst, m_hDC))
		return false;

	// 입력관리자를 초기화한다.
	if (!GET_SINGLE(CInput)->Init(m_hWnd))
		return false;

	// 충돌 관리자를 초기화한다.
	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

	// 오브젝트 관리자를 초기화한다.
	if (!GET_SINGLE(CObjectManager)->Init())
		return false;

	// 장면관리자를 초기화한다.
	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	// 백버퍼를 얻어온다.
	m_pBackBuffer = GET_SINGLE(CResourcesManager)->FindTexture("BackBuffer");

	return true;
}

int CCore::Run()
{
	MSG msg;

	while (m_bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우 데드타임일 때는 else  구문으로 들어온다.
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CCore::Logic()
{
	CTimer*	pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainTimer");

	pTimer->Update();

	Input(pTimer->GetDeltaTime());
	Update(pTimer->GetDeltaTime());
	Collision(pTimer->GetDeltaTime());
	Render(pTimer->GetDeltaTime());
}

void CCore::Input(float fTime)
{
	// 입력 관리자 업데이트
	GET_SINGLE(CInput)->Update(fTime);

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		GET_SINGLE(CCamera)->SetTarget(NULL);

	GET_SINGLE(CCamera)->Input(fTime);
	GET_SINGLE(CSceneManager)->Input(fTime);
}

void CCore::Update(float fTime)
{
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

	/*static POINT	ptStart, ptEnd;
	static bool		bStart;

	if (KEYDOWN("MouseLButton"))
	{
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
		HDC	hBackDC = m_pBackBuffer->GetMemDC();
		MoveToEx(hBackDC, ptStart.x, ptStart.y, NULL);
		POINT	ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(m_hWnd, &ptCursor);

		LineTo(hBackDC, ptCursor.x, ptStart.y);
		LineTo(hBackDC, ptCursor.x, ptCursor.y);
		LineTo(hBackDC, ptStart.x, ptCursor.y);
		LineTo(hBackDC, ptStart.x, ptStart.y);
	}*/

	// 마지막으로 마우스를 백버퍼에 그린다.
	GET_SINGLE(CInput)->GetMouse()->Render(m_pBackBuffer->GetMemDC(), fTime);

	// 마지막으로 백버퍼를 화면에 그린다.
	BitBlt(m_hDC, 0, 0, m_iWidth, m_iHeight, m_pBackBuffer->GetMemDC(),
		0, 0, SRCCOPY);
}

ATOM CCore::WindowRegisterClass(TCHAR * pClass, int iIconID)
{
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

	return RegisterClassExW(&wcex);
}

BOOL CCore::InitWindow(TCHAR* pClass, TCHAR * pTitle, UINT iWidth, UINT iHeight)
{
	HWND hWnd = CreateWindowW(pClass, pTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, iWidth, iHeight, nullptr, nullptr, m_hInst, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	// 윈도우의 클라이언트 영역을 원하는 크기로 설정하는 방법
	RECT	rc = { 0, 0, iWidth, iHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	m_hWnd = hWnd;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:	// 키를 누를때 들어오는 메세지이다.
		switch (wParam)
		{
		case VK_ESCAPE:
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
