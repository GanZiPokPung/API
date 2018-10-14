#pragma once
#include "Obj.h"
class CUIRadioButton :
	public CObj
{
protected:
	friend class CObjectManager;

protected:
	CUIRadioButton();
	CUIRadioButton(const CUIRadioButton& button);
	virtual ~CUIRadioButton();

protected:
	//CHECKBUTTON_STATE	m_eState;
	bool				IsChecked;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CUIRadioButton* Clone();

public:
	void CollisionEnter(class CCollider* p1, class CCollider* p2);
	void CollisionTrigger(class CCollider* p1, class CCollider* p2);
	void CollisionLeave(class CCollider* p1, class CCollider* p2);
};

