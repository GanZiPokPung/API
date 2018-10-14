#pragma once
#include "Collider.h"
class CColliderPoint :
	public CCollider
{
private:
	CColliderPoint();
	CColliderPoint(const CColliderPoint& point);
	~CColliderPoint();

private:
	POSITION	m_tPos;

public:
	POSITION GetPoint()	const
	{
		return m_tPos;
	}

public:
	static CColliderPoint* Create(const string& strTag, const POSITION& tPos);

public:
	virtual void Move(const POSITION& tMove);
	virtual void Move(float x, float y);
	virtual void Move(const POINT& ptMove);

public:
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CColliderPoint* Clone();

public:
	virtual bool Collision(CCollider* pCollider);
};

