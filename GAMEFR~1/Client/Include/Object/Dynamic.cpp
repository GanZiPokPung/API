#include "Dynamic.h"
#include "../Core/Math.h"

CDynamic::CDynamic()
{
}

CDynamic::CDynamic(const CDynamic & dynamic)	:
	CObj(dynamic)
{
	m_fSpeed = dynamic.m_fSpeed;
	m_fAngle = dynamic.m_fAngle;
}

CDynamic::~CDynamic()
{
}

void CDynamic::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CDynamic::SetAngle(float fAngle)
{
	m_fAngle = fAngle;
}

void CDynamic::Move(float fTime)
{
	float	fSpeed = m_fSpeed * fTime;
	m_tPos.x += cosf(CMath::AngleToRadian(m_fAngle)) * fSpeed;
	m_tPos.y += sinf(CMath::AngleToRadian(m_fAngle)) * fSpeed;
}

bool CDynamic::Init()
{
	m_fSpeed = 100.f;
	m_fAngle = 0.f;

	return true;
}

void CDynamic::Input(float fTime)
{
}

void CDynamic::Update(float fTime)
{
	CObj::Update(fTime);
}

void CDynamic::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);
}
