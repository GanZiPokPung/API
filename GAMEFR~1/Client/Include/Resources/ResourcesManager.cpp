#include "ResourcesManager.h"
#include "Texture.h"

DEFINITION_SINGLE(CResourcesManager)

CResourcesManager::CResourcesManager()
{
}

CResourcesManager::~CResourcesManager()
{
	// ���� �ߴ� ������ Release�Ͽ� RefCnt�� ����
	Safe_Release_Map(m_mapTexture);
}

bool CResourcesManager::Init(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;

	// BackBuffer�� �� Bmp������ ������´�.
	// un_map�� insert
	CTexture*	pTexture = LoadTexture("BackBuffer", L"BackBuffer.bmp");

	SAFE_RELEASE(pTexture);

	return true;
}

CTexture * CResourcesManager::LoadTexture(const string & strKey, TCHAR * pFileName,
	bool bColorKey, COLORREF dwColorKey, const string & strPathKey)
{
	// Ű���� �̹� �ִ��� Ȯ���Ѵ�.
	// ������ nullptr
	CTexture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;

	// �ؽ�ó�� �ҷ��ͼ�
	if (!pTexture->LoadTexture(m_hInst, m_hDC, pFileName, bColorKey,
		dwColorKey, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	// ���� Ref �ö�
	pTexture->AddRef();
	// un_map�� insert 
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CTexture * CResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, CTexture*>::iterator	iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end())
		return NULL;

	// ���� Ref �ö�
	iter->second->AddRef();

	return iter->second;
}
