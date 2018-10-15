#include "SceneManager.h"
#include "Scene.h"
#include "MainScene.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_pCurScene(NULL)
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_pCurScene);
}

bool CSceneManager::Init()
{
	// 씬을 만들어 씬 포인터 타입 데이터 멤버에 집어 넣음
	m_pCurScene = CreateScene();
	// MainScene을 Create한다
	m_pCurScene->CreateSceneScript<CMainScene>();

	return true;
}

int CSceneManager::Input(float fTime)
{
	// Input
	m_pCurScene->Input(fTime);

	return 0;
}

int CSceneManager::Update(float fTime)
{
	// Update
	m_pCurScene->Update(fTime);

	return 0;
}

void CSceneManager::Render(HDC hDC, float fTime)
{
	// Render
	m_pCurScene->Render(hDC, fTime);
}

CScene * CSceneManager::CreateScene()
{
	// Scene을 초기화 하고 만든 동적할당한 인스턴스를 반환
	CScene* pScene = new CScene;

	if (!pScene->Init())
	{
		SAFE_DELETE(pScene);
		return NULL;
	}

	return pScene;
}
