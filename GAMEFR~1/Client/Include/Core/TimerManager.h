#pragma once

#include "../GameFramework.h"

class CTimerManager
{
private:
	// map : Red Tree�� ����Ͽ� Key�� ������ �����ȴ�.
	// unordered_map : Hash Table�� ����Ͽ� key�� ������ ������ �ʿ䰡 ����
	//				  Ž�� �ӵ��� ������ ������ ������.

	unordered_map<string, class CTimer*>	m_mapTimer;

public:
	bool Init();
	class CTimer* CreateTimer(const string& strKey);
	class CTimer* FindTimer(const string& strKey);

	DECLARE_SINGLE(CTimerManager)
};

