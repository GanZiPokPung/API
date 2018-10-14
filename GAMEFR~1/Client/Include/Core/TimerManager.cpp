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

	// Timer를 만들고 초기화 한다.
	// 만약 실패한다면 nullptr를 반환한다.
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
	// find는 알맞는 key값을 찾지 못했다면 .end()위치의 iterator를 리턴한다.
	// const auto&로도 사용 가능
	unordered_map<string, CTimer*>::iterator	iter = m_mapTimer.find(strKey);

	// iterator가 .end()일때
	if (iter == m_mapTimer.end())
		return NULL;

	return iter->second;
}
