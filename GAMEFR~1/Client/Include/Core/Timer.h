#pragma once

#include "../GameFramework.h"

class CTimer
{
private:
	friend class CTimerManager;

private:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER	m_tPrev;
	LARGE_INTEGER	m_tSecond;
	bool			m_bStartUpdate;
	float			m_fDeltaTime;

	float			m_fFPS;
	float			m_fFPSTime;
	UINT			m_iFrame;
	UINT			m_iFrameLimit;

public:
	float GetDeltaTime()	const
	{
		return m_fDeltaTime;
	}
	
public:
	bool Init();
	void Update();
	void Destroy();
};

