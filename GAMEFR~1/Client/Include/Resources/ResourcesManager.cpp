#include "ResourcesManager.h"
#include "Texture.h"

DEFINITION_SINGLE(CResourcesManager)

CResourcesManager::CResourcesManager()
{
}

CResourcesManager::~CResourcesManager()
{
	Safe_Release_Map(m_mapTexture);
}

bool CResourcesManager::Init(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;

	CTexture*	pTexture = LoadTexture("BackBuffer", L"BackBuffer.bmp");

	SAFE_RELEASE(pTexture);

	return true;
}

CTexture * CResourcesManager::LoadTexture(const string & strKey, TCHAR * pFileName,
	bool bColorKey, COLORREF dwColorKey, const string & strPathKey)
{
	CTexture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, pFileName, bColorKey,
		dwColorKey, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CTexture * CResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, CTexture*>::iterator	iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
