#pragma once
#include "Dynamic.h"
class CEffect :
	public CDynamic
{
private:
	friend class CObjectManager;

protected:
	CEffect();
	CEffect(const CEffect& effect);
	~CEffect();

private:
	float	m_fLifeTime;

public:
	void SetLifeTime(float fLifeTime);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CEffect* Clone();
};

