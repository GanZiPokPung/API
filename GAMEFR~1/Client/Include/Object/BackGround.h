#pragma once
#include "Obj.h"
class CBackGround :
	public CObj
{
protected:
	friend class CObjectManager;

protected:
	CBackGround();
	CBackGround(const CBackGround& back);
	virtual ~CBackGround();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CBackGround* Clone();
};

