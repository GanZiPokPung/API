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
	m_pCurScene = CreateScene();

	m_pCurScene->CreateSceneScript<CMainScene>();

	return true;
}

int CSceneManager::Input(float fTime)
{
	m_pCurScene->Input(fTime);

	return 0;
}

int CSceneManager::Update(float fTime)
{
	m_pCurScene->Update(fTime);

	return 0;
}

void CSceneManager::Render(HDC hDC, float fTime)
{
	m_pCurScene->Render(hDC, fTime);
}

CScene * CSceneManager::CreateScene()
{
	CScene* pScene = new CScene;

	if (!pScene->Init())
	{
		SAFE_DELETE(pScene);
		return NULL;
	}

	return pScene;
}
