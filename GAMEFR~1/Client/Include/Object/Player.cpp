#include "Player.h"
#include "../Resources/Texture.h"
#include "../Animation/Animation.h"
#include "../Core/Input.h"
#include "Bullet.h"
#include "ObjectManager.h"
#include "../Scene/Layer.h"
#include "../Core.h"
#include "../Collider/ColliderPixel.h"
#include "../Core/Camera.h"

CPlayer::CPlayer()
{
	m_iHP = 1000000;
}

CPlayer::CPlayer(const CPlayer & player)	:
	CCharacter(player)
{
	m_fFireTime = player.m_fFireTime;
	m_fFireTimeLimit = player.m_fFireTimeLimit;
	m_eAnimation = player.m_eAnimation;
	m_iHP = player.m_iHP;
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPos(RESOLUTION_W / 2.f, RESOLUTION_H / 2.f);
	SetPivot(0.5f, 1.f);

	// 이미지를 지정한다.
	SetTexture("Player", L"Left.bmp", true);

	SetAnimation("player.anm");

	SetSize(m_pAnimation->GetFrameWidth(), m_pAnimation->GetFrameHeight());
	/*
	SetAnimation("PlayerAni", AT_LINE, AO_LOOP,
		m_pTexture->GetWidth(), m_pTexture->GetHeight(), 21, 8, 1.f);

	m_pAnimation->AddLineFrameCount(5);
	m_pAnimation->AddLineFrameCount(14);
	m_pAnimation->AddLineFrameCount(5);
	m_pAnimation->AddLineFrameCount(21);
	m_pAnimation->AddLineFrameCount(12);
	m_pAnimation->AddLineFrameCount(8);
	m_pAnimation->AddLineFrameCount(4);
	m_pAnimation->AddLineFrameCount(5);
	*/

	SetSpeed(200.f);

	GET_SINGLE(CInput)->CreateKey(VK_SPACE, "Fire");

	m_fFireTime = 0.f;
	m_fFireTimeLimit = 1.f;
	m_fSkill1Time = 0.f;

	m_eAnimation = PA_STATIC;
	m_bCancelKey = false;

	m_pAnimation->ChangeAnimation(m_eAnimation);

	return true;
}

void CPlayer::Input(float fTime)
{
	CCharacter::Input(fTime);

	float	fSpeed = m_fSpeed * fTime;

	//m_eAnimation = PA_STATIC;
	bool	bPush = false;
	
	if (KEYDOWN("MoveUp") || KEYPUSH("MoveUp"))
	{
		m_tPos.y -= fSpeed;
		m_eAnimation = PA_RUN;
		bPush = true;
		m_bCancelKey = false;
	}

	if (KEYDOWN("MoveDown") || KEYPUSH("MoveDown"))
	{
		m_tPos.y += fSpeed;
		m_eAnimation = PA_RUN;
		bPush = true;
		m_bCancelKey = false;
	}

	if (KEYDOWN("MoveLeft") || KEYPUSH("MoveLeft"))
	{
		m_tPos.x -= fSpeed;
		m_eAnimation = PA_RUN;
		bPush = true;
		m_bCancelKey = false;
	}

	if (KEYDOWN("MoveRight") || KEYPUSH("MoveRight"))
	{
		m_tPos.x += fSpeed;
		m_eAnimation = PA_RUN;
		bPush = true;
		m_bCancelKey = false;
	}
	
	if (KEYDOWN("Skill1"))
	{
		m_fSkill1Time = 5.f;
		m_fFireTimeLimit /= 2.f;
	}

	m_fFireTime += fTime;

	if ((KEYDOWN("Fire") || KEYPUSH("Fire")) &&
		m_fFireTime >= m_fFireTimeLimit)
	{
		m_fFireTime = 0.f;
		m_eAnimation = PA_SHOW;
		bPush = true;
		m_bCancelKey = true;
		CreateBullet();
	}

	if (KEYDOWN("Jump") || KEYPUSH("Jump"))
	{
		//m_tPos.y -= 30;
		m_eAnimation = PA_FLY;
		bPush = true;
		m_bCancelKey = false;
		m_bJump = true;
	}

	if (m_bCancelKey)
	{
		if (m_pAnimation->GetAnimationFrameEnd())
		{
			m_eAnimation = PA_STATIC;
			m_bCancelKey = false;
		}
	}

	else if(!bPush)
	{
		m_eAnimation = PA_STATIC;
	}

	m_pAnimation->ChangeAnimation(m_eAnimation);
}

void CPlayer::Update(float fTime)
{
	CCharacter::Update(fTime);

	if (m_fSkill1Time > 0.f)
	{
		m_fSkill1Time -= fTime;

		if (m_fSkill1Time <= 0.f)
		{
			m_fSkill1Time = 0.f;
			m_fFireTimeLimit *= 2.f;
		}
	}
}

void CPlayer::Render(HDC hDC, float fTime)
{
	CCharacter::Render(hDC, fTime);
	POSITION	tPos = m_tPos - GET_SINGLE(CCamera)->GetPos();
	TCHAR	strHP[32] = {};
	wsprintf(strHP, L"HP : %d", m_iHP);
	TextOut(hDC, tPos.x, tPos.y - m_tSize.y, strHP, lstrlen(strHP));
}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::CreateBullet()
{
	CObj*	pBullet = GET_SINGLE(CObjectManager)->CreateCloneObject("Bullet");

	float	x, y;
	x = m_tPos.x + m_tSize.x * (1.f - m_tPivot.x) + pBullet->GetSize().x *
		pBullet->GetPivot().x;
	y = m_tPos.y + m_tSize.y * (0.5f - m_tPivot.y);
	pBullet->SetPos(x, y);

	m_pLayer->AddObject(pBullet);

	SAFE_RELEASE(pBullet);
}

void CPlayer::CollisionEnter(CCollider * pColl1, CCollider * pColl2)
{
	if (pColl2->GetTag() == "Bullet")
	{
		m_iHP -= 5;
	}
}

void CPlayer::CollisionTrigger(CCollider * pColl1, CCollider * pColl2)
{
	if (pColl2->GetTag() == "MapCollider")
	{
		OutputDebugString(L"픽셀 충돌\n");
		PPIXELCOLLIDERINFO	pInfo = ((CColliderPixel*)pColl2)->GetPixelInfo();
		//InitGravityTime();
		m_tPos.y = pInfo->tIntersectPos.y;
		m_bJump = false;
		m_fJumpAccel = 0.f;
	}
}
