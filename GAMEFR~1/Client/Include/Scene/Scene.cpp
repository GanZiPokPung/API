#include "Scene.h"
#include "Layer.h"
#include "SceneScript.h"

CScene::CScene()	:
	m_pSceneScript(NULL)
{
}

CScene::~CScene()
{
	SAFE_DELETE(m_pSceneScript);
	Safe_Delete_VecList(m_vecLayer);
}

bool CScene::Init()
{
	CreateLayer("MapLayer", 0);
	CreateLayer("DefaultLayer", 1);
	CreateLayer("UILayer", INT_MAX);

	return true;
}

void CScene::Input(float fTime)
{
	if(m_pSceneScript)
		m_pSceneScript->Input(fTime);

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Input(fTime);
	}
}

void CScene::Update(float fTime)
{
	if (m_pSceneScript)
		m_pSceneScript->Update(fTime);

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fTime);
	}
}

void CScene::Render(HDC hDC, float fTime)
{
	if (m_pSceneScript)
		m_pSceneScript->Render(hDC, fTime);

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fTime);
	}
}

void CScene::Destroy()
{
	delete	this;
}

CLayer * CScene::CreateLayer(const string & strName, int iZOrder)
{
	CLayer*	pLayer = new CLayer;

	if (!pLayer->Init(strName, iZOrder))
	{
		SAFE_DELETE(pLayer);
		return NULL;
	}

	pLayer->SetScene(this);

	m_vecLayer.push_back(pLayer);

	if(m_vecLayer.size() >= 2)
		sort(m_vecLayer.begin(), m_vecLayer.end(), CScene::LayerSort);

	return pLayer;
}

CLayer * CScene::FindLayer(const string & strName)
{
	for (size_t i = 0; i < m_vecLayer.size(); ++i)
	{
		if (m_vecLayer[i]->GetName() == strName)
			return m_vecLayer[i];
	}

	return NULL;
}

bool CScene::LayerSort(CLayer * p1, CLayer * p2)
{
	return p1->GetZOrder() < p2->GetZOrder();
}
