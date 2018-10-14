#pragma once
#include "Dynamic.h"
class CBullet :
	public CDynamic
{
private:
	friend class CObjectManager;

protected:
	CBullet();
	CBullet(const CBullet& bullet);
	~CBullet();

private:
	float	m_fDist;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CBullet* Clone();

public:
	void CollisionLeave(class CCollider* pColl1, class CCollider* pColl2);
};

