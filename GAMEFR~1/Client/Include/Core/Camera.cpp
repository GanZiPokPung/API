#include "Camera.h"
#include "Input.h"
#include "../Object/Obj.h"
#include "../Core.h"

DEFINITION_SINGLE(CCamera)

CCamera::CCamera()	:
	m_pTarget(NULL)
{
}

CCamera::~CCamera()
{
	SAFE_RELEASE(m_pTarget);
}

void CCamera::SetResolution(float x, float y)
{
	m_tResolution.x = x;
	m_tResolution.y = y;
}

void CCamera::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CCamera::SetTarget(CObj * pTarget)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pTarget;
	if(m_pTarget)
		m_pTarget->AddRef();
}

bool CCamera::Init(float x, float y, float cx, float cy)
{
	m_tPos.x = x;
	m_tPos.y = y;

	m_tResolution.x = cx;
	m_tResolution.y = cy;

	m_fSpeed = 200.f;

	m_tTargetPivot.x = 0.7f;
	m_tTargetPivot.y = 0.7f;

	return true;
}

void CCamera::Input(float fTime)
{
	if (m_pTarget)
		return;

	float	fSpeed = m_fSpeed * fTime;

	if (KEYDOWN("MoveUp") || KEYPUSH("MoveUp"))
	{
		m_tPos.y -= fSpeed;

		if (m_tPos.y <= 0.f)
			m_tPos.y = 0.f;
	}

	if (KEYDOWN("MoveDown") || KEYPUSH("MoveDown"))
	{
		m_tPos.y += fSpeed;

		if (m_tPos.y >= m_tResolution.y - RESOLUTION_H)
			m_tPos.y = m_tResolution.y - RESOLUTION_H;
	}

	if (KEYDOWN("MoveLeft") || KEYPUSH("MoveLeft"))
	{
		m_tPos.x -= fSpeed;

		if (m_tPos.x <= 0.f)
			m_tPos.x = 0.f;
	}

	if (KEYDOWN("MoveRight") || KEYPUSH("MoveRight"))
	{
		m_tPos.x += fSpeed;

		if (m_tPos.x >= m_tResolution.x - RESOLUTION_W)
			m_tPos.x = m_tResolution.x - RESOLUTION_W;
	}
}

void CCamera::Update(float fTime)
{
	if (m_pTarget)
	{
		POSITION	tPos = m_pTarget->GetPos();

		float	fRight = m_tResolution.x - RESOLUTION_W * (1.f - m_tTargetPivot.x);
		float	fBottom = m_tResolution.y - RESOLUTION_H * (1.f - m_tTargetPivot.y);

		if (tPos.x < RESOLUTION_W * m_tTargetPivot.x)
			m_tPos.x = 0.f;

		else if (tPos.x > fRight)
			m_tPos.x = m_tResolution.x - RESOLUTION_W;
		
		else
			m_tPos.x = tPos.x - RESOLUTION_W * m_tTargetPivot.x;

		if (tPos.y < RESOLUTION_H * m_tTargetPivot.y)
			m_tPos.y = 0.f;

		else if (tPos.y > fBottom)
			m_tPos.y = m_tResolution.y - RESOLUTION_H;

		else
			m_tPos.y = tPos.y - RESOLUTION_H * m_tTargetPivot.y;
	}
}
