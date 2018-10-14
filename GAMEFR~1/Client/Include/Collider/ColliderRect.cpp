#include "ColliderRect.h"
#include "../Core/Camera.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"
#include "../Object/Obj.h"

CColliderRect::CColliderRect()
{
	m_eType = CT_RECT;
}

CColliderRect::CColliderRect(const CColliderRect & rc)	:
	CCollider(rc)
{
	m_tRC = rc.m_tRC;
}

CColliderRect::~CColliderRect()
{
}

CColliderRect * CColliderRect::Create(const string & strTag, float fLeft,
	float fTop, float fRight, float fBottom)
{
	CColliderRect*	pRC = new CColliderRect;

	if (!pRC->Init())
	{
		SAFE_DELETE(pRC);
		return NULL;
	}

	pRC->SetTag(strTag);
	pRC->SetRect(fLeft, fTop, fRight, fBottom);

	return pRC;
}

void CColliderRect::SetRect(float fLeft, float fTop, float fRight, 
	float fBottom)
{
	m_tRC.fLeft = fLeft;
	m_tRC.fTop = fTop;
	m_tRC.fRight = fRight;
	m_tRC.fBottom = fBottom;
}

void CColliderRect::Move(const POSITION & tMove)
{
	m_tRC.fLeft += tMove.x;
	m_tRC.fRight += tMove.x;
	m_tRC.fTop += tMove.y;
	m_tRC.fBottom += tMove.y;
}

void CColliderRect::Move(float x, float y)
{
	m_tRC.fLeft += x;
	m_tRC.fRight += x;
	m_tRC.fTop += y;
	m_tRC.fBottom += y;
}

void CColliderRect::Move(const POINT & ptMove)
{
	m_tRC.fLeft += ptMove.x;
	m_tRC.fRight += ptMove.x;
	m_tRC.fTop += ptMove.y;
	m_tRC.fBottom += ptMove.y;
}

bool CColliderRect::Init()
{
	return true;
}

void CColliderRect::Update(float fTime)
{
	CCollider::Update(fTime);

	// 이 아래에 Rect 충돌체의 개별적인 기능을 구현한다.
	Move(m_tMove);
}

void CColliderRect::Render(HDC hDC, float fTime)
{
#ifdef _DEBUG
	POSITION	tCamPos = GET_SINGLE(CCamera)->GetPos();

	RECTANGLE	rc = m_tRC;

	if (m_pObj->GetCameraEnable())
		rc -= tCamPos;

	MoveToEx(hDC, rc.fLeft, rc.fTop, NULL);
	LineTo(hDC, rc.fRight, rc.fTop);
	LineTo(hDC, rc.fRight, rc.fBottom);
	LineTo(hDC, rc.fLeft, rc.fBottom);
	LineTo(hDC, rc.fLeft, rc.fTop);
#endif // _DEBUG
}

CColliderRect * CColliderRect::Clone()
{
	return new CColliderRect(*this);
}

bool CColliderRect::Collision(CCollider * pCollider)
{
	// 들어온 충돌체의 타입을 판단한다.
	switch (pCollider->GetType())
	{
	case CT_RECT:
		return CollisionRectToRect(m_tRC, ((CColliderRect*)pCollider)->m_tRC);
	case CT_SPHERE:
		return CollisionSphereToRect(((CColliderSphere*)pCollider)->GetSphere(), m_tRC);
	case CT_LINE:
		break;
	case CT_POINT:
		return CollisionRectToPoint(m_tRC, ((CColliderPoint*)pCollider)->GetPoint());
	case CT_PIXEL:
		return CollisionPixelToRect(((CColliderPixel*)pCollider)->GetPixelInfo(), m_tRC);
	}

	return false;
}
