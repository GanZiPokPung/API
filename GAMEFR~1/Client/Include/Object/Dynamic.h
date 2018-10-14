#pragma once
#include "Obj.h"
class CDynamic :
	public CObj
{
protected:
	CDynamic();
	CDynamic(const CDynamic& dynamic);
	virtual ~CDynamic() = 0;

protected:
	float		m_fSpeed;
	float		m_fAngle;

public:
	void SetSpeed(float fSpeed);
	void SetAngle(float fAngle);
	void Move(float fTime);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CDynamic* Clone() = 0;
};

