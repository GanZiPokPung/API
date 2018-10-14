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

