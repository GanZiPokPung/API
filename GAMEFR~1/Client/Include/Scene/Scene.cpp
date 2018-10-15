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
	// MapLayer, DefaultLayer, UILayer를 나누어 만듬
	// UI가 가장 마지막에 와야 하기 때문에 INT_MAX
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


	// Layer 순회돌며 Input 메서드 호출
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

	// Layer 순회돌며 Update 메서드 호출
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

	// Layer 순회돌며 Render 메서드 호출
	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fTime);
	}
}

void CScene::Destroy()
{
	// Scene 삭제
	delete	this;
}

CLayer * CScene::CreateLayer(const string & strName, int iZOrder)
{
	// Layer 인스턴스를 동적할당하고 초기화 하여
	CLayer*	pLayer = new CLayer;

	if (!pLayer->Init(strName, iZOrder))
	{
		SAFE_DELETE(pLayer);
		return NULL;
	}

	// Layer에 Scene을 Set
	pLayer->SetScene(this);

	// Vector형 Layer 데이터 멤버에 push_back 한다.
	m_vecLayer.push_back(pLayer);

	// Vector의 size가 2개 이상일때 추가 할때마다 정렬함
	if(m_vecLayer.size() >= 2)
		sort(m_vecLayer.begin(), m_vecLayer.end(), CScene::LayerSort);

	return pLayer;
}

CLayer * CScene::FindLayer(const string & strName)
{
	// 인덱스 접근을 통해 Layer를 찾음
	for (size_t i = 0; i < m_vecLayer.size(); ++i)
	{
		if (m_vecLayer[i]->GetName() == strName)
			return m_vecLayer[i];
	}

	return NULL;
}

bool CScene::LayerSort(CLayer * p1, CLayer * p2)
{
	// 오름차순
	// Z값의 오름순으로 정렬한다.
	return p1->GetZOrder() < p2->GetZOrder();
}
