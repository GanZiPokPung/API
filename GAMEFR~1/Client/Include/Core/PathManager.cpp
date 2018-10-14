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
	// ���������� �ִ� ��θ� ���´�.
	TCHAR	strPath[MAX_PATH] = {};

	//���� .exe���� ��θ� �ҷ��´�.
	GetModuleFileName(NULL, strPath, MAX_PATH);

	for (int i = lstrlen(strPath); i > 0; --i)
	{
		//�ǵں��� \�� /�� �����ٸ�
		if (strPath[i - 1] == '\\' || strPath[i - 1] == '/')
		{
			//�� �ڿ� Client.exe�� �����.
			memset(&strPath[i], 0, sizeof(TCHAR) * (MAX_PATH - i));
			break;
		}
	}

	//Base ��ġ�� ROOT_PATH�� ���Ͽ� map�� �ִ´�.
	m_mapPath.insert(make_pair(ROOT_PATH, strPath));

	//Texture�� Animation ��θ� �����Ͽ� map�� �ִ´�.
	CreatePath(TEXTURE_PATH, L"Textures\\");
	CreatePath(ANIMATION_PATH, L"Animation\\");

	return true;
}

bool CPathManager::CreatePath(const string & strKey, TCHAR * pPath, 
	const string & strBaseKey)
{
	//�̹� �����ϴ� key���̸� false
	if (FindPath(strKey))
		return false;

	// �⺻ �н� ������ ���´�.
	// ROOT_PATH += pPath
	wstring	strPath = FindPath(strBaseKey);
	strPath += pPath;

	// map�� insert�Ѵ�.
	m_mapPath.insert(make_pair(strKey, strPath));

	return true;
}

const wchar_t * CPathManager::FindPath(const string & strKey)
{
	unordered_map<string, wstring>::iterator	iter = m_mapPath.find(strKey);

	if (iter == m_mapPath.end())
		return NULL;

	// c_str() const wchar_t �������� ��ȯ(�����ڵ� ����)
	return iter->second.c_str();
}

wstring CPathManager::FindPathFromWString(const string & strKey)
{
	unordered_map<string, wstring>::iterator	iter = m_mapPath.find(strKey);

	if (iter == m_mapPath.end())
		return L"";

	// wstring(�����ڵ�) ��ȯ
	return iter->second;
}

const char * CPathManager::FindPathFromMultibyte(const string & strKey)
{
	const wchar_t* strFindPath = FindPath(strKey);

	if (!strFindPath)
		return NULL;

	char	strPath[MAX_PATH] = {};

	//
	WideCharToMultiByte(CP_ACP, 0, strFindPath, -1, /*��ȯ�� ���ڿ��� ����. ���� -1�� ���� ���� -1�� �ش� ���ڿ� ��ü�� ����*/
		strPath, lstrlen(strFindPath) + 1, NULL, NULL);

	m_strChangePath = strPath;

	return m_strChangePath.c_str();
}
