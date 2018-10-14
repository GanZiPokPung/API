#include "Monster.h"
#include "../Animation/Animation.h"
#include "ObjectManager.h"
#include "Bullet.h"
#include "../Core/Math.h"
#include "../Scene/Layer.h"
#include "../Collider/Collider.h"

CMonster::CMonster()
{
}

CMonster::CMonster(const CMonster & monster)	:
	CDynamic(monster)
{
	m_eAnimation = monster.m_eAnimation;
	m_fFireTimeLimit = monster.m_fFireTimeLimit;
	m_fFireTime = 0.f;
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	if (!CDynamic::Init())
		return false;

	SetPos(1000.f, 500.f);
	SetSize(20.f, 20.f);
	SetPivot(0.5f, 1.f);

	// 이미지를 지정한다.
	SetTexture("Monster", L"Monster_Sword2_Left.bmp", true, RGB(86, 177, 222));

	SetAnimation("Monster.anm");

	SetSize(m_pAnimation->GetFrameWidth(), m_pAnimation->GetFrameHeight());
	
	SetSpeed(300.f);

	m_eAnimation = MA_WALK;

	m_pAnimation->ChangeAnimation(m_eAnimation);

	m_fFireTime = 0.f;
	m_fFireTimeLimit = 2.f;

	return true;
}

void CMonster::Input(float fTime)
{
	CDynamic::Input(fTime);
}

void CMonster::Update(float fTime)
{
	CDynamic::Update(fTime);

	if (m_eAnimation == MA_ATTACK)
	{
		if (m_pAnimation->GetAnimationFrameEnd())
		{
			m_fFireTime = 0.f;
			m_eAnimation = MA_WALK;
			m_pAnimation->ChangeAnimation(m_eAnimation);
			CreateBullet();
		}
	}

	m_fFireTime += fTime;

	if (m_fFireTime >= m_fFireTimeLimit &&
		m_eAnimation != MA_ATTACK)
	{
		m_eAnimation = MA_ATTACK;

		m_pAnimation->ChangeAnimation(m_eAnimation);
	}
}

void CMonster::Render(HDC hDC, float fTime)
{
	CDynamic::Render(hDC, fTime);
}

CMonster * CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::CreateBullet()
{
	// 발사할 총알을 생성한다.
	CBullet*	pBullet = (CBullet*)GET_SINGLE(CObjectManager)->CreateCloneObject("Bullet");

	// 총알의 위치를 설정한다.
	float	x, y;
	x = m_tPos.x - m_tSize.x * (1.f - m_tPivot.x) - pBullet->GetSize().x *
		pBullet->GetPivot().x;
	y = m_tPos.y + m_tSize.y * (0.5f - m_tPivot.y);
	pBullet->SetPos(x, y);

	// 충돌체를 얻어온다.
	CCollider*	pColl = pBullet->FindCollider("Bullet");

	pColl->AddCollisionFunction<CBullet>(CFT_ENTER, &CBullet::CollisionLeave, pBullet);

	CObj*	pTarget = m_pLayer->FindObjectFromTag("Player");

	pBullet->SetAngle(CMath::GetAngle(pBullet->GetPos(), pTarget->GetPos()));

	SAFE_RELEASE(pTarget);

	m_pLayer->AddObject(pBullet);

	SAFE_RELEASE(pBullet);
}
