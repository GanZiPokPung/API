#include "Bullet.h"
#include "../Collider/Collider.h"
#include "ObjectManager.h"
#include "../Scene/Layer.h"

CBullet::CBullet()
{
}

CBullet::CBullet(const CBullet & bullet)	:
	CDynamic(bullet)
{
	m_fDist = bullet.m_fDist;
}

CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	if (!CDynamic::Init())
		return false;

	m_fDist = 700.f;

	SetPos(100.f, 100.f);
	SetSize(20.f, 20.f);
	SetPivot(0.5f, 0.5f);

	// 이미지를 지정한다.
	SetTexture("Bullet", L"Pistol.bmp", true, RGB(0, 248, 0));

	SetSpeed(300.f);

	return true;
}

void CBullet::Input(float fTime)
{
	CDynamic::Input(fTime);
}

void CBullet::Update(float fTime)
{
	CDynamic::Update(fTime);

	Move(fTime);

	m_fDist -= m_fSpeed * fTime;

	if (m_fDist <= 0.f)
		Die();
}

void CBullet::Render(HDC hDC, float fTime)
{
	CDynamic::Render(hDC, fTime);
}

CBullet * CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::CollisionLeave(CCollider * pColl1, CCollider * pColl2)
{
	if (pColl2->GetTag() == "PlayerBody")
	{
		CObj*	pFireEffect = GET_SINGLE(CObjectManager)->CreateCloneObject("FireEffect");

		pFireEffect->SetPos(m_tPos);
		pFireEffect->SetPivot(0.5f, 1.f);

		m_pLayer->AddObject(pFireEffect);

		SAFE_RELEASE(pFireEffect);

		Die();
	}
}
