#pragma once
#include "Dynamic.h"

enum MONSTER_ANIMATION
{
	MA_ATTACK,
	MA_WALK
};

class CMonster :
	public CDynamic
{
private:
	friend class CObjectManager;

protected:
	CMonster();
	CMonster(const CMonster& monster);
	~CMonster();

private:
	MONSTER_ANIMATION	m_eAnimation;
	float				m_fFireTime;
	float				m_fFireTimeLimit;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CMonster* Clone();

private:
	void CreateBullet();
};

