#pragma once

#include "../GameFramework.h"

class CPathManager
{
private:
	unordered_map<string, wstring>	m_mapPath;
	string		m_strChangePath;

public:
	bool Init();
	bool CreatePath(const string& strKey, TCHAR* pPath,
		const string& strBaseKey = ROOT_PATH);
	const wchar_t* FindPath(const string& strKey);
	wstring FindPathFromWString(const string& strKey);
	const char* FindPathFromMultibyte(const string& strKey);

	DECLARE_SINGLE(CPathManager)
};

