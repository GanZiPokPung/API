#pragma once

#include "../GameFramework.h"

class CCollisionManager
{
private:
	vector<class CObj*>		m_vecObj;

public:
	bool Init();
	void AddObject(class CObj* pObj);
	void Collision(float fTime);

private:
	void CollisionObj(class CObj* pObj1, class CObj* pObj2);

	DECLARE_SINGLE(CCollisionManager)
};

