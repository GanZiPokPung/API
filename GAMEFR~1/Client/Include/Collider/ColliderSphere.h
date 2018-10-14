#pragma once

#include "Collider.h"

class CColliderSphere :
	public CCollider
{
private:
	CColliderSphere();
	CColliderSphere(const CColliderSphere& sphere);
	~CColliderSphere();

private:
	SPHERE	m_tSphere;

public:
	SPHERE GetSphere()	const
	{
		return m_tSphere;
	}

public:
	static CColliderSphere* Create(const string& strTag, float x, float y, float r);
	static CColliderSphere* Create(const string& strTag, const POSITION& tCenter, float r);

public:
	void SetSphere(const POSITION& tCenter, float r);
	void SetSphere(float x, float y, float r);

public:
	virtual void Move(const POSITION& tMove);
	virtual void Move(float x, float y);
	virtual void Move(const POINT& ptMove);

public:
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CColliderSphere* Clone();

public:
	virtual bool Collision(CCollider* pCollider);
};

