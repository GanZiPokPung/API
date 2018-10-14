#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
}

bool CPathManager::Init()
{
	// 실행파일이 있는 경로를 얻어온다.
	TCHAR	strPath[MAX_PATH] = {};

	//현재 .exe파일 경로를 불러온다.
	GetModuleFileName(NULL, strPath, MAX_PATH);

	for (int i = lstrlen(strPath); i > 0; --i)
	{
		//맨뒤부터 \나 /를 만난다면
		if (strPath[i - 1] == '\\' || strPath[i - 1] == '/')
		{
			//맨 뒤에 Client.exe를 지운다.
			memset(&strPath[i], 0, sizeof(TCHAR) * (MAX_PATH - i));
			break;
		}
	}

	//Base 위치를 ROOT_PATH로 정하여 map에 넣는다.
	m_mapPath.insert(make_pair(ROOT_PATH, strPath));

	//Texture와 Animation 경로를 설정하여 map에 넣는다.
	CreatePath(TEXTURE_PATH, L"Textures\\");
	CreatePath(ANIMATION_PATH, L"Animation\\");

	return true;
}

bool CPathManager::CreatePath(const string & strKey, TCHAR * pPath, 
	const string & strBaseKey)
{
	//이미 존재하는 key값이면 false
	if (FindPath(strKey))
		return false;

	// 기본 패스 정보를 얻어온다.
	// ROOT_PATH += pPath
	wstring	strPath = FindPath(strBaseKey);
	strPath += pPath;

	// map에 insert한다.
	m_mapPath.insert(make_pair(strKey, strPath));

	return true;
}

const wchar_t * CPathManager::FindPath(const string & strKey)
{
	unordered_map<string, wstring>::iterator	iter = m_mapPath.find(strKey);

	if (iter == m_mapPath.end())
		return NULL;

	// c_str() const wchar_t 형식으로 반환(유니코드 형식)
	return iter->second.c_str();
}

wstring CPathManager::FindPathFromWString(const string & strKey)
{
	unordered_map<string, wstring>::iterator	iter = m_mapPath.find(strKey);

	if (iter == m_mapPath.end())
		return L"";

	// wstring(유니코드) 반환
	return iter->second;
}

const char * CPathManager::FindPathFromMultibyte(const string & strKey)
{
	const wchar_t* strFindPath = FindPath(strKey);

	if (!strFindPath)
		return NULL;

	char	strPath[MAX_PATH] = {};

	//
	WideCharToMultiByte(CP_ACP, 0, strFindPath, -1, /*변환할 문자열의 길이. 보통 -1을 많이 쓰고 -1은 해당 문자열 전체를 지정*/
		strPath, lstrlen(strFindPath) + 1, NULL, NULL);

	m_strChangePath = strPath;

	return m_strChangePath.c_str();
}
