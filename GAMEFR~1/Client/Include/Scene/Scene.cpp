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
	// MapLayer, DefaultLayer, UILayer�� ������ ����
	// UI�� ���� �������� �;� �ϱ� ������ INT_MAX
	CreateLayer("MapLayer", 0);
	CreateLayer("DefaultLayer", 1);
	CreateLayer("UILayer", INT_MAX);

	return true;
}

void CScene::Input(float fTime)
{
	// SceneScript
	if(m_pSceneScript)
		m_pSceneScript->Input(fTime);


	// Layer ��ȸ���� Input �޼��� ȣ��
	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Input(fTime);
	}
}

void CScene::Update(float fTime)
{
	// SceneScript
	if (m_pSceneScript)
		m_pSceneScript->Update(fTime);

	// Layer ��ȸ���� Update �޼��� ȣ��
	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fTime);
	}
}

void CScene::Render(HDC hDC, float fTime)
{
	// SceneScript
	if (m_pSceneScript)
		m_pSceneScript->Render(hDC, fTime);

	// Layer ��ȸ���� Render �޼��� ȣ��
	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fTime);
	}
}

void CScene::Destroy()
{
	// Scene ����
	delete	this;
}

CLayer * CScene::CreateLayer(const string & strName, int iZOrder)
{
	// Layer �ν��Ͻ��� �����Ҵ��ϰ� �ʱ�ȭ �Ͽ�
	CLayer*	pLayer = new CLayer;

	if (!pLayer->Init(strName, iZOrder))
	{
		SAFE_DELETE(pLayer);
		return NULL;
	}

	// Layer�� Scene�� Set
	pLayer->SetScene(this);

	// Vector�� Layer ������ ����� push_back �Ѵ�.
	m_vecLayer.push_back(pLayer);

	// Vector�� size�� 2�� �̻��϶� �߰� �Ҷ����� ������
	if(m_vecLayer.size() >= 2)
		sort(m_vecLayer.begin(), m_vecLayer.end(), CScene::LayerSort);

	return pLayer;
}

CLayer * CScene::FindLayer(const string & strName)
{
	// �ε��� ������ ���� Layer�� ã��
	for (size_t i = 0; i < m_vecLayer.size(); ++i)
	{
		if (m_vecLayer[i]->GetName() == strName)
			return m_vecLayer[i];
	}

	return NULL;
}

bool CScene::LayerSort(CLayer * p1, CLayer * p2)
{
	// ��������
	// Z���� ���������� �����Ѵ�.
	return p1->GetZOrder() < p2->GetZOrder();
}
