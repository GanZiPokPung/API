#include "Effect.h"
#include "../Animation/Animation.h"

CEffect::CEffect()
{
}

CEffect::CEffect(const CEffect & effect)	:
	CDynamic(effect)
{
	m_fLifeTime = effect.m_fLifeTime;
}

CEffect::~CEffect()
{
}

void CEffect::SetLifeTime(float fLifeTime)
{
	m_fLifeTime = fLifeTime;
}

bool CEffect::Init()
{
	if (!CDynamic::Init())
		return false;

	m_fLifeTime = 1.f;
	m_fSpeed = 0.f;

	return true;
}

void CEffect::Input(float fTime)
{
	CDynamic::Input(fTime);
}

void CEffect::Update(float fTime)
{
	CDynamic::Update(fTime);

	Move(fTime);

	if (m_pAnimation->GetAnimationFrameEnd())
	{
		if (m_pAnimation->GetAnimationInfo().eOption == AO_ONCE)
			Die();
	}

	m_fLifeTime -= fTime;

	if (m_fLifeTime <= 0.f)
		Die();
}

void CEffect::Render(HDC hDC, float fTime)
{
	CDynamic::Render(hDC, fTime);
}

CEffect * CEffect::Clone()
{
	return new CEffect(*this);
}
