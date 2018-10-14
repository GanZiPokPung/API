#pragma once

#include "../GameFramework.h"

class CCamera
{
private:
	POSITION	m_tPos;
	_SIZE		m_tResolution;
	float		m_fSpeed;
	class CObj*	m_pTarget;
	POSITION	m_tTargetPivot;

public:
	POSITION GetPos()	const
	{
		return m_tPos;
	}

	_SIZE GetResolution()	const
	{
		return m_tResolution;
	}

public:
	void SetResolution(float x, float y);
	void SetSpeed(float fSpeed);
	void SetTarget(class CObj* pTarget);

public:
	bool Init(float x, float y, float cx, float cy);
	void Input(float fTime);
	void Update(float fTime);

	DECLARE_SINGLE(CCamera)
};

