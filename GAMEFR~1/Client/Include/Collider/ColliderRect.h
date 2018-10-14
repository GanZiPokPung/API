#pragma once
#include "Collider.h"
class CColliderRect :
	public CCollider
{
private:
	CColliderRect();
	CColliderRect(const CColliderRect& rc);
	~CColliderRect();
	
private:
	RECTANGLE	m_tRC;

public:
	RECTANGLE GetRect()	const
	{
		return m_tRC;
	}

public:
	static CColliderRect* Create(const string& strTag, float fLeft, float fTop, float fRight,
		float fBottom);

public:
	void SetRect(float fLeft, float fTop, float fRight,
		float fBottom);

public:
	virtual void Move(const POSITION& tMove);
	virtual void Move(float x, float y);
	virtual void Move(const POINT& ptMove);

public:
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CColliderRect* Clone();

public:
	virtual bool Collision(CCollider* pCollider);
};

