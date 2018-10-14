#pragma once
#include "Obj.h"
class CUIButton :
	public CObj
{
protected:
	friend class CObjectManager;

protected:
	CUIButton();
	CUIButton(const CUIButton& button);
	virtual ~CUIButton();

protected:
	BUTTON_STATE	m_eState;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CUIButton* Clone();

public:
	void CollisionEnter(class CCollider* p1, class CCollider* p2);
	void CollisionTrigger(class CCollider* p1, class CCollider* p2);
	void CollisionLeave(class CCollider* p1, class CCollider* p2);
};

