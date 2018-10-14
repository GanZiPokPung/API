#include "ColliderSphere.h"
#include "../Core/Camera.h"
#include "../Core/Math.h"
#include "ColliderRect.h"
#include "ColliderPoint.h"
#include "../Object/Obj.h"

CColliderSphere::CColliderSphere()
{
	m_eType = CT_SPHERE;
}

CColliderSphere::CColliderSphere(const CColliderSphere & sphere)	:
	CCollider(sphere)
{
	m_tSphere = sphere.m_tSphere;
}

CColliderSphere::~CColliderSphere()
{
}

CColliderSphere * CColliderSphere::Create(const string & strTag, float x, float y, float r)
{
	CColliderSphere*	pSphere = new CColliderSphere;

	if (!pSphere->Init())
	{
		SAFE_DELETE(pSphere);
		return NULL;
	}

	pSphere->SetTag(strTag);
	pSphere->SetSphere(x, y, r);

	return pSphere;
}

CColliderSphere * CColliderSphere::Create(const string & strTag, const POSITION & tCenter, float r)
{
	CColliderSphere*	pSphere = new CColliderSphere;

	if (!pSphere->Init())
	{
		SAFE_DELETE(pSphere);
		return NULL;
	}

	pSphere->SetTag(strTag);
	pSphere->SetSphere(tCenter, r);

	return pSphere;
}

void CColliderSphere::SetSphere(const POSITION & tCenter, float r)
{
	m_tSphere.tCenter = tCenter;
	m_tSphere.fRadius = r;
}

void CColliderSphere::SetSphere(float x, float y, float r)
{
	m_tSphere.tCenter.x = x;
	m_tSphere.tCenter.y = y;
	m_tSphere.fRadius = r;
}

void CColliderSphere::Move(const POSITION & tMove)
{
	m_tSphere += tMove;
}

void CColliderSphere::Move(float x, float y)
{
	m_tSphere.tCenter.x += x;
	m_tSphere.tCenter.y += y;
}

void CColliderSphere::Move(const POINT & ptMove)
{
	m_tSphere += ptMove;
}

bool CColliderSphere::Init()
{
	return true;
}

void CColliderSphere::Update(float fTime)
{
	CCollider::Update(fTime);

	// 이 아래에 Rect 충돌체의 개별적인 기능을 구현한다.
	Move(m_tMove);
}

void CColliderSphere::Render(HDC hDC, float fTime)
{
#ifdef _DEBUG
	POSITION	tCamPos = GET_SINGLE(CCamera)->GetPos();

	SPHERE	tSphere = m_tSphere;

	if (m_pObj->GetCameraEnable())
		tSphere -= tCamPos;

	POSITION	tPos;
	tPos.x = tSphere.tCenter.x + cosf(CMath::AngleToRadian(0.f)) * tSphere.fRadius;
	tPos.y = tSphere.tCenter.y + sinf(CMath::AngleToRadian(0.f)) * tSphere.fRadius;
	
	MoveToEx(hDC, tPos.x, tPos.y, NULL);
	
	for (int i = 1; i <= 360; ++i)
	{
		tPos.x = tSphere.tCenter.x + cosf(CMath::AngleToRadian(i)) * tSphere.fRadius;
		tPos.y = tSphere.tCenter.y + sinf(CMath::AngleToRadian(i)) * tSphere.fRadius;

		LineTo(hDC, tPos.x, tPos.y);
	}

	//Ellipse(hDC, tSphere.tCenter.x - tSphere.fRadius, tSphere.tCenter.y - tSphere.fRadius,
	//	tSphere.tCenter.x + tSphere.fRadius, tSphere.tCenter.y + tSphere.fRadius);
#endif // _DEBUG
}

CColliderSphere * CColliderSphere::Clone()
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider * pCollider)
{
	// 들어온 충돌체의 타입을 판단한다.
	switch (pCollider->GetType())
	{
	case CT_RECT:
		return CollisionSphereToRect(m_tSphere, ((CColliderRect*)pCollider)->GetRect());
	case CT_SPHERE:
		return CollisionSphereToSphere(m_tSphere, ((CColliderSphere*)pCollider)->m_tSphere);
	case CT_LINE:
		break;
	case CT_POINT:
		break;
	case CT_PIXEL:
		break;
	}

	return false;
}
