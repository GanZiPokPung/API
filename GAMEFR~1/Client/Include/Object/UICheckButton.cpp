#include "UICheckButton.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Input.h"


CUICheckButton::CUICheckButton() :
	IsChecked(false)
{
	SetTag("CheckButton");
}

CUICheckButton::CUICheckButton(const CUICheckButton & button)
{
	IsChecked = false;
}


CUICheckButton::~CUICheckButton()
{
}

bool CUICheckButton::Init()
{

	IsChecked = false;
	DisableCamera();

	CColliderRect* pRC = CColliderRect::Create("CheckButton", -50.f, -50.f, 50.f, 50.f);

	AddCollider(pRC);

	pRC->AddCollisionFunction<CUICheckButton>(CFT_ENTER, &CUICheckButton::CollisionEnter, this);
	pRC->AddCollisionFunction<CUICheckButton>(CFT_TRIGGER, &CUICheckButton::CollisionTrigger, this);
	pRC->AddCollisionFunction<CUICheckButton>(CFT_LEAVE, &CUICheckButton::CollisionLeave, this);


	return true;
}

void CUICheckButton::Input(float fTime)
{
}

void CUICheckButton::Update(float fTime)
{
	CObj::Update(fTime);
}

void CUICheckButton::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);

	//if (!IsChecked)
	//{
	//	TextOut(hDC, this->m_tPos.x - 50, this->m_tPos.y, L"체크해제", 4);
	//}
	//else
	//{
	//	TextOut(hDC, this->m_tPos.x - 50, this->m_tPos.y, L"체크됨", 2);
	//}

	//switch (IsChecked)
	//{
	//case CS_UNCHECKED:
	//	TextOut(hDC, 0, 0, L"체크해제", 5);
	//	break;
	//case CS_CHECKED:
	//	TextOut(hDC, 0, 0, L"체크됨", 2);
	//	break;
	//}
}

CUICheckButton * CUICheckButton::Clone()
{
	return new CUICheckButton(*this);
}

void CUICheckButton::CollisionEnter(CCollider * p1, CCollider * p2)
{
}

void CUICheckButton::CollisionTrigger(CCollider * p1, CCollider * p2)
{
	// 체크 언체크
	if (p2->GetTag() == "Mouse")
	{
		if (KEYDOWN("MouseLButton"))
		{
			IsChecked = !IsChecked;
		}
	}
	if (!IsChecked)
	{
		SetTexture("CheckButton", L"CheckButton.bmp");
	}
	else
	{
		SetTexture("CheckButtonCheck", L"CheckButtonCheck.bmp");
	}
}

void CUICheckButton::CollisionLeave(CCollider * p1, CCollider * p2)
{
}
