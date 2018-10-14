#include "Obj.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Animation/Animation.h"
#include "../Core/Camera.h"
#include "../Core.h"
#include "../Collider/Collider.h"

CObj::CObj() :
	m_pTexture(NULL),
	m_pAnimation(NULL),
	m_bEnable(true),
	m_bAlive(true),
	m_bUseGravity(false),

	m_fGravityTime(0.f),
	m_bJump(false),
	m_fJumpAccel(0.f),

	m_bCamera(true)

{
	m_tPivot = POSITION(0.5f, 0.5f);
}

CObj::CObj(const CObj & obj)
{
	// 얕은복사
	*this = obj;
	m_iRefCount = 1;
	m_fGravityTime = 0.f;
	m_fJumpAccel = 0.f;
	if (m_pTexture)
		m_pTexture->AddRef();

	if (m_pAnimation)
		m_pAnimation = m_pAnimation->Clone();

	m_ColliderList.clear();

	list<CCollider*>::const_iterator	iter;
	list<CCollider*>::const_iterator	iterEnd = obj.m_ColliderList.end();

	for (iter = obj.m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		CCollider*	pCollider = (*iter)->Clone();
		pCollider->SetObj(this);
		m_ColliderList.push_back(pCollider);
	}
}

CObj::~CObj()
{
	Safe_Delete_VecList(m_ColliderList);
	SAFE_DELETE(m_pAnimation);
	SAFE_RELEASE(m_pTexture);
}

CTexture * CObj::GetTexture() const
{
	if (m_pTexture)
		m_pTexture->AddRef();
	return m_pTexture;
}

CAnimation * CObj::GetAnimation() const
{
	return m_pAnimation;
}

void CObj::SetSizeFromImageSize()
{
	if (!m_pTexture)
		return;

	m_tSize.x = m_pTexture->GetWidth();
	m_tSize.y = m_pTexture->GetHeight();
}

void CObj::SetSizeFromAnimationFrameSize()
{
	if (!m_pAnimation)
		return;

	m_tSize.x = m_pAnimation->GetFrameWidth();
	m_tSize.y = m_pAnimation->GetFrameHeight();
}

void CObj::SetAnimationFrameSize()
{
	if (!m_pAnimation)
		return;

	m_tSize.x = m_pAnimation->GetFrameWidth();
	m_tSize.y = m_pAnimation->GetFrameHeight();
}

void CObj::SetScene(CScene * pScene)
{
	m_pScene = pScene;

	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetScene(m_pScene);
	}
}

void CObj::SetLayer(CLayer * pLayer)
{
	m_pLayer = pLayer;

	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetLayer(m_pLayer);
	}
}

void CObj::SetTexture(CTexture * pTexture)
{
	m_pTexture = pTexture;
	m_pTexture->AddRef();

	if (m_pAnimation)
	{
		m_pAnimation->SetTextureSize(m_pTexture->GetWidth(), m_pTexture->GetHeight());
	}
}

void CObj::SetTexture(const string & strKey, TCHAR * pFileName, bool bColorKey,
	COLORREF dwColorKey, const string & strPathKey)
{
	if (pFileName)
		m_pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, bColorKey, dwColorKey, strPathKey);

	else
		m_pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strKey);

	if (m_pAnimation)
	{
		m_pAnimation->SetTextureSize(m_pTexture->GetWidth(), m_pTexture->GetHeight());
	}
}

void CObj::SetAnimation(CAnimation * pAnimation)
{
	SAFE_DELETE(m_pAnimation);
	m_pAnimation = pAnimation;

	if (m_pTexture)
	{
		m_pAnimation->SetTextureSize(m_pTexture->GetWidth(), m_pTexture->GetHeight());
	}
}

void CObj::SetAnimation(const char * pName, ANIMATION_TYPE eType, 
	ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight, UINT iFrameMaxX,
	UINT iFrameMaxY, float fLimitTime)
{
	SAFE_DELETE(m_pAnimation);
	m_pAnimation = CAnimation::Create(pName, eType, eOption, iWidth, iHeight,
		iFrameMaxX, iFrameMaxY, fLimitTime);
}

void CObj::SetAnimation(const char * pFileName, const string & strPathKey)
{
	SAFE_DELETE(m_pAnimation);
	m_pAnimation = CAnimation::Create(pFileName, strPathKey);

	if (m_pTexture)
	{
		m_pAnimation->SetTextureSize(m_pTexture->GetWidth(), m_pTexture->GetHeight());
	}
}

void CObj::AddCollider(CCollider * pCollider)
{
	pCollider->SetScene(m_pScene);
	pCollider->SetLayer(m_pLayer);
	pCollider->SetObj(this);
	m_ColliderList.push_back(pCollider);
}

bool CObj::CheckCollider()
{
	return !m_ColliderList.empty();
}

CCollider * CObj::FindCollider(const string & strTag)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
			return *iter;
	}

	return NULL;
}

void CObj::Update(float fTime)
{
	if (m_pAnimation)
		m_pAnimation->Update(fTime);

	// 중력 업데이트
	if (m_bUseGravity)
	{
		m_fGravityTime += fTime;
		float	fMoveY = GRAVITY * (m_fGravityTime * m_fGravityTime);

		if (fMoveY >= 1.f)
			fMoveY = 1.f;

		m_tPos.y += fMoveY;
	}

	if (m_bJump)
	{
		float m_fAccel = -5.f + m_fJumpAccel;

		if(m_fAccel < -2.5f)
			m_fJumpAccel += 0.05f;
		else
			m_fJumpAccel += 0.025f;

		if (m_fAccel >= 0)
			m_fAccel = 0;
		m_tPos.y += m_fAccel;
	}

	// 충돌체 업데이트
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fTime);
	}
}

void CObj::Render(HDC hDC, float fTime)
{
	POSITION	tPos = m_tPos;
	
	if (m_bCamera)
		tPos -= GET_SINGLE(CCamera)->GetPos();

	else
		int a = 10;

	if (tPos.x + (1.f - m_tPivot.x) * m_tSize.x < 0 ||
		tPos.x - m_tPivot.x * m_tSize.x > RESOLUTION_W ||
		tPos.y + (1.f - m_tPivot.y) * m_tSize.y < 0 ||
		tPos.y - m_tPivot.y * m_tSize.y > RESOLUTION_H)
		return;

	if (m_pTexture)
	{
		if (!m_pTexture->GetColorKeyEnable())
		{
			// BitBlt 함수는 특정 DC를 대상이 되는 DC에 출력해주는 함수이다.
			// 1번 인자는 대상이 되는 DC로 여기 DC에다가 출력하게 된다.
			// 2번, 3번 인자는 x, y인데 이 값은 출력 대상의 DC에서 좌표를 지정해준다.
			// 좌표는 좌 상단 점이 된다.
			// 4번, 5번 인자는 가로, 세로의 출력 크기이다.
			// 2, 3번 인자의 좌표로부터 4번, 5번 인자의 가로, 세로 크기만큼
			// 6번인자에 들어오는 DC를 그려낸다.
			// 7, 8번 인자는 이미지의 출력 시작점을 설정한다.
			int	x, y;
			x = tPos.x - m_tPivot.x * m_tSize.x;
			y = tPos.y - m_tPivot.y * m_tSize.y;
			BitBlt(hDC, x, y, m_pTexture->GetWidth(), m_pTexture->GetHeight(),
				m_pTexture->GetMemDC(), 0, 0,
				SRCCOPY);
		}

		else
		{
			// ColorKey를 제외하고 출력한다.
			int	x, y;
			x = tPos.x - m_tPivot.x * m_tSize.x;
			y = tPos.y - m_tPivot.y * m_tSize.y;

			UINT	iWidth, iHeight;
			iWidth = m_pTexture->GetWidth();
			iHeight = m_pTexture->GetHeight();

			UINT	iImageX = 0, iImageY = 0;

			if (m_pAnimation)
			{
				iWidth = m_pAnimation->GetFrameWidth();
				iHeight = m_pAnimation->GetFrameHeight();

				iImageX = m_pAnimation->GetAnimationInfo().iFrameX * iWidth;
				iImageY = m_pAnimation->GetAnimationInfo().iFrameY * iHeight;
			}

			TransparentBlt(hDC, x, y, iWidth, iHeight,
				m_pTexture->GetMemDC(), iImageX, iImageY, iWidth, iHeight,
				m_pTexture->GetColorKey());
		}
	}

	// 충돌체 업데이트
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fTime);
	}
}
