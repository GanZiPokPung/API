#pragma once
#include "Character.h"

enum PLAYER_ANIMATION
{
	PA_STATIC,
	PA_BYE,
	PA_FLY,
	PA_SHOW,
	PA_WALK,
	PA_RUN,
	PA_STAND,
	PA_ENTANGLE,
	PA_END
};

class CPlayer :
	public CCharacter
{
private:
	friend class CObjectManager;

protected:
	CPlayer();
	CPlayer(const CPlayer& player);
	virtual ~CPlayer();

private:
	float				m_fFireTime;
	float				m_fFireTimeLimit;
	PLAYER_ANIMATION	m_eAnimation;
	bool				m_bCancelKey;
	int					m_iHP;

private:
	float				m_fSkill1Time;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CPlayer* Clone();

private:
	void CreateBullet();

public:
	void CollisionEnter(class CCollider* pColl1, class CCollider* pColl2);
	void CollisionTrigger(class CCollider* pColl1, class CCollider* pColl2);
};

