#include "ColliderPoint.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "ColliderRect.h"

CColliderPoint::CColliderPoint()
{
	m_eType = CT_POINT;
}

CColliderPoint::CColliderPoint(const CColliderPoint & point)	:
	CCollider(point)
{
	m_tPos = point.m_tPos;
}

CColliderPoint::~CColliderPoint()
{
}

CColliderPoint * CColliderPoint::Create(const string & strTag, const POSITION & tPos)
{
	CColliderPoint*	pPoint = new CColliderPoint;

	if (!pPoint->Init())
	{
		SAFE_DELETE(pPoint);
		return NULL;
	}

	pPoint->m_tPos = tPos;
	pPoint->SetTag(strTag);

	return pPoint;
}

void CColliderPoint::Move(const POSITION & tMove)
{
	m_tPos += tMove;
}

void CColliderPoint::Move(float x, float y)
{
	m_tPos.x += x;
	m_tPos.y += y;
}

void CColliderPoint::Move(const POINT & ptMove)
{
	m_tPos += ptMove;
}

bool CColliderPoint::Init()
{
	return true;
}

void CColliderPoint::Update(float fTime)
{
	CCollider::Update(fTime);

	// 이 아래에 Rect 충돌체의 개별적인 기능을 구현한다.
	Move(m_tMove);
}

void CColliderPoint::Render(HDC hDC, float fTime)
{
}

CColliderPoint * CColliderPoint::Clone()
{
	return new CColliderPoint(*this);
}

bool CColliderPoint::Collision(CCollider * pCollider)
{
	// 들어온 충돌체의 타입을 판단한다.
	switch (pCollider->GetType())
	{
	case CT_RECT:
		return CollisionRectToPoint(((CColliderRect*)pCollider)->GetRect(), m_tPos);
	case CT_SPHERE:
		break;
	case CT_LINE:
		break;
	case CT_POINT:
		return m_tPos == ((CColliderPoint*)pCollider)->m_tPos;
	case CT_PIXEL:
		break;
	}

	return false;
}
