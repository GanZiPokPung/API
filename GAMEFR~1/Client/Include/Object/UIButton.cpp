#include "UIButton.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Input.h"

CUIButton::CUIButton()
{
	SetTag("Button");
}

CUIButton::CUIButton(const CUIButton & button)	:
	CObj(button)
{
	m_eState = BS_NONE;
}

CUIButton::~CUIButton()
{
}

bool CUIButton::Init()
{
	m_eState = BS_NONE;
	DisableCamera();

	CColliderRect* pRC = CColliderRect::Create("Button", -50.f, -50.f, 50.f, 50.f);
	
	AddCollider(pRC);

	pRC->AddCollisionFunction<CUIButton>(CFT_ENTER, &CUIButton::CollisionEnter, this);
	pRC->AddCollisionFunction<CUIButton>(CFT_TRIGGER, &CUIButton::CollisionTrigger, this);
	pRC->AddCollisionFunction<CUIButton>(CFT_LEAVE, &CUIButton::CollisionLeave, this);


	return true;
}

void CUIButton::Input(float fTime)
{
}

void CUIButton::Update(float fTime)
{
	CObj::Update(fTime);
}

void CUIButton::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);

	switch (m_eState)
	{
	case BS_NONE:
		TextOut(hDC, this->m_tPos.x - 50, this->m_tPos.y, L"일반", 2);
		break;
	case BS_MOUSEON:
		TextOut(hDC, this->m_tPos.x - 50, this->m_tPos.y, L"마우스오버", 5);
		break;
	case BS_CLICK:
		TextOut(hDC, this->m_tPos.x - 50, this->m_tPos.y, L"클릭", 2);
		break;
	}
}

CUIButton * CUIButton::Clone()
{
	return new CUIButton(*this);
}

void CUIButton::CollisionEnter(CCollider * p1, CCollider * p2)
{
	if (p2->GetTag() == "Mouse")
		m_eState = BS_MOUSEON;
}

void CUIButton::CollisionTrigger(CCollider * p1, CCollider * p2)
{
	if (p2->GetTag() == "Mouse")
	{
		m_eState = BS_MOUSEON;

		if (KEYDOWN("MouseLButton") || KEYPUSH("MouseLButton"))
		{
			m_eState = BS_CLICK;
		}
	}
}

void CUIButton::CollisionLeave(CCollider * p1, CCollider * p2)
{
	m_eState = BS_NONE;
}
