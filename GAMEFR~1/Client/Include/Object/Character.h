#pragma once
#include "Dynamic.h"
class CCharacter :
	public CDynamic
{
protected:
	friend class CObjectManager;

protected:
	CCharacter();
	CCharacter(const CCharacter& character);
	virtual ~CCharacter();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CCharacter* Clone();
};

