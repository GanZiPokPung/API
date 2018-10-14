#pragma once

#include "Obj.h"

class CUICheckButton :
	public CObj
{
protected:
	friend class CObjectManager;

protected:
	CUICheckButton();
	CUICheckButton(const CUICheckButton& button);
	virtual ~CUICheckButton();

protected:
	//CHECKBUTTON_STATE	m_eState;
	bool				IsChecked;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CUICheckButton* Clone();

public:
	void CollisionEnter(class CCollider* p1, class CCollider* p2);
	void CollisionTrigger(class CCollider* p1, class CCollider* p2);
	void CollisionLeave(class CCollider* p1, class CCollider* p2);
};

