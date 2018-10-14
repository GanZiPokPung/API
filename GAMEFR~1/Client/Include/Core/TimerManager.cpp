#include "TimerManager.h"
#include "Timer.h"

DEFINITION_SINGLE(CTimerManager)

CTimerManager::CTimerManager()
{
}

CTimerManager::~CTimerManager()
{
	Safe_Delete_Map(m_mapTimer);
}

bool CTimerManager::Init()
{
	CreateTimer("MainTimer");

	return true;
}

CTimer * CTimerManager::CreateTimer(const string & strKey)
{
	// 먼저 같은 키를 사용하는 타이머가 있는지를 체크해서 있을 경우 해당 타이머를
	// 반환해준다.
	CTimer*	pTimer = FindTimer(strKey);

	if (pTimer)
		return pTimer;

	pTimer = new CTimer;

	if (!pTimer->Init())
	{
		SAFE_DELETE(pTimer);
		return NULL;
	}

	m_mapTimer.insert(make_pair(strKey, pTimer));

	return pTimer;
}

CTimer * CTimerManager::FindTimer(const string & strKey)
{
	unordered_map<string, CTimer*>::iterator	iter = m_mapTimer.find(strKey);

	if (iter == m_mapTimer.end())
		return NULL;

	return iter->second;
}
