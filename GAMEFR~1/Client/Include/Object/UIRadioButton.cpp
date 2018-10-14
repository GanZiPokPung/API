#include "UIRadioButton.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Input.h"


CUIRadioButton::CUIRadioButton() :
	IsChecked(false)
{
	SetTag("RadioButton");
}

CUIRadioButton::CUIRadioButton(const CUIRadioButton & button)
{
	IsChecked = false;
}


CUIRadioButton::~CUIRadioButton()
{

}

bool CUIRadioButton::Init()
{

	IsChecked = false;
	DisableCamera();

	CColliderRect* pRC = CColliderRect::Create("RadioButton", -50.f, -50.f, 50.f, 50.f);

	AddCollider(pRC);

	pRC->AddCollisionFunction<CUIRadioButton>(CFT_ENTER, &CUIRadioButton::CollisionEnter, this);
	pRC->AddCollisionFunction<CUIRadioButton>(CFT_TRIGGER, &CUIRadioButton::CollisionTrigger, this);
	pRC->AddCollisionFunction<CUIRadioButton>(CFT_LEAVE, &CUIRadioButton::CollisionLeave, this);


	return true;
}

void CUIRadioButton::Input(float fTime)
{
}

void CUIRadioButton::Update(float fTime)
{
	CObj::Update(fTime);
}

void CUIRadioButton::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);

	//if (!IsChecked)
	//{
	//	TextOut(hDC, this->m_tPos.x-50, this->m_tPos.y, L"체크해제", 4);
	//}
	//else
	//{
	//	TextOut(hDC, this->m_tPos.x-50, this->m_tPos.y, L"체크됨", 2);
	//}
}

CUIRadioButton * CUIRadioButton::Clone()
{
	return new CUIRadioButton(*this);;
}

void CUIRadioButton::CollisionEnter(CCollider * p1, CCollider * p2)
{
}

void CUIRadioButton::CollisionTrigger(CCollider * p1, CCollider * p2)
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
		SetTexture("RadioButton", L"RadioButton.bmp");
	}
	else
	{
		SetTexture("RadioButtonCheck", L"RadioButtonCheck.bmp");
	}
}

void CUIRadioButton::CollisionLeave(CCollider * p1, CCollider * p2)
{
}
