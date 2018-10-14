#pragma once

#include "GameFramework.h"

class CCore
{
private:
	CCore();
	~CCore();

private:
	static CCore*	m_pInst;

public:
	static CCore* GetInst()
	{
		// m_pInst가 NULL일 경우 동적할당 해준다.
		// NULL이 아닐 경우 그냥 반환한다. m_pInst는 static 멤버 변수이므로 클래스 차원에서
		// 접근이 가능하므로 CCore클래스를 이용해서 생성한 모든 객체가 공유하는 변수이기 때
		// 문에 한번 생성하면 다음부터는 생성된 녀석을 동일하게 반환받아 사용하게 된다.
		if (!m_pInst)
			m_pInst = new CCore;
		return m_pInst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_pInst);
	}

private:
	static bool	m_bLoop;

private:
	/* HWND : 핸들 윈도우 약자, 윈도우의 핸들번호를 저장해서 사용, 하나의 프로그램에서 많은양의 윈도우를 띄울수 있다.
				즉 윈도우창의 번호들(HWND) 로 구분하는것이다.
	   HINSTANCE : 핸들 인스턴스, 프로그램의 인스턴스 식별자, 쉽게 보면 프로그램 자체의 실체화된 주소.
	   결론 : HINSTANCE는 프로그램 자체의 핸들이며, HWND는 프로그램안의 윈도우창의 번호 !!*/

	/*Handle Device Context(HDC)
		Device Context 란 ?
			출력에 필요한 정보를 가지는 데이터 구조체, 좌표, 색, 굵기등 출력에 필요한 모든 정보를 담고있다
		- 결국 HDC 란 DC의 정보를 저장하는 데이터 구조체의 위치를 알기위함.
		- HDC는 포인터가 아니다.항상 실제 객체의 메모리 주소를 가르킬 뿐이다.
		->물리적으로 메모리 주소가 바뀌어도, DC의 실제위치를 찾아감.*/
	HINSTANCE		m_hInst;
	HWND			m_hWnd;
	UINT			m_iWidth;
	UINT			m_iHeight;
	HDC				m_hDC;
	class CTexture*	m_pBackBuffer;

public:
	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HDC GetDC()	const
	{
		return m_hDC;
	}

	UINT GetWidth()	const
	{
		return m_iWidth;
	}

	UINT GetHeight()	const
	{
		return m_iHeight;
	}

public:
	bool Init(HINSTANCE hInst, TCHAR* pTitle, TCHAR* pClass, int iIconID,
		UINT iWidth, UINT iHeight);
	int Run();

private:
	void Logic();
	void Input(float fTime);
	void Update(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

private:
	ATOM WindowRegisterClass(TCHAR* pClass, int iIconID);
	BOOL InitWindow(TCHAR* pClass, TCHAR* pTitle, UINT iWidth, UINT iHeight);

public:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

