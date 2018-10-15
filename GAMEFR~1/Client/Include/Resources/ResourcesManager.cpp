#include "ResourcesManager.h"
#include "Texture.h"

DEFINITION_SINGLE(CResourcesManager)

CResourcesManager::CResourcesManager()
{
}

CResourcesManager::~CResourcesManager()
{
	// 참조 했던 값들을 Release하여 RefCnt를 감소
	Safe_Release_Map(m_mapTexture);
}

bool CResourcesManager::Init(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;

	// BackBuffer로 쓸 Bmp파일을 가지고온다.
	// un_map에 insert
	CTexture*	pTexture = LoadTexture("BackBuffer", L"BackBuffer.bmp");

	SAFE_RELEASE(pTexture);

	return true;
}

CTexture * CResourcesManager::LoadTexture(const string & strKey, TCHAR * pFileName,
	bool bColorKey, COLORREF dwColorKey, const string & strPathKey)
{
	// 키값이 이미 있는지 확인한다.
	// 없으면 nullptr
	CTexture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;

	// 텍스처를 불러와서
	if (!pTexture->LoadTexture(m_hInst, m_hDC, pFileName, bColorKey,
		dwColorKey, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	// 참조 Ref 올라감
	pTexture->AddRef();
	// un_map에 insert 
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CTexture * CResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, CTexture*>::iterator	iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end())
		return NULL;

	// 참조 Ref 올라감
	iter->second->AddRef();

	return iter->second;
}
