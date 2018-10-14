#pragma once
#include "Obj.h"
class CMouse :
	public CObj
{
private:
	friend class CGameObject;
	friend class CInput;

private:
	CMouse();
	CMouse(const CMouse& mouse);
	~CMouse();

private:
	POINT		m_ptMousePos;
	POINT		m_ptMouseMove;
	HWND		m_hWnd;

public:
	void SetWindowHandle(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

public:
	POINT GetMousePos()	const
	{
		return m_ptMousePos;
	}

	POINT GetMouseMove()	const
	{
		return m_ptMouseMove;
	}

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CMouse* Clone();

public:
	void CollisionEnter(class CCollider* p1, class CCollider* p2);
	void CollisionTrigger(class CCollider* p1, class CCollider* p2);
	void CollisionLeave(class CCollider* p1, class CCollider* p2);
};

