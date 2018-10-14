#pragma once

#include "../GameFramework.h"

class CTimerManager
{
private:
	// map : Red Tree를 사용하여 Key의 순서가 유지된다.
	// unordered_map : Hash Table을 사용하여 key의 순서를 유지할 필요가 없음
	//				  탐색 속도에 유리한 이점을 가진다.

	unordered_map<string, class CTimer*>	m_mapTimer;

public:
	bool Init();
	class CTimer* CreateTimer(const string& strKey);
	class CTimer* FindTimer(const string& strKey);

	DECLARE_SINGLE(CTimerManager)
};

