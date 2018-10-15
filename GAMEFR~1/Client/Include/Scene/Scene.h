#pragma once

#include "../GameFramework.h"

class CScene
{
private:
	CScene();
	~CScene();

private:
	friend class CSceneManager;

private:
	// Layer
	vector<class CLayer*>	m_vecLayer;
	// SceneScript
	class CSceneScript*		m_pSceneScript;

public:
	bool Init();
	void Input(float fTime);
	void Update(float fTime);
	void Render(HDC hDC, float fTime);
	void Destroy();
	class CLayer* CreateLayer(const string& strName, int iZOrder);
	class CLayer* FindLayer(const string& strName);

public:
	template <typename T>
	bool CreateSceneScript()
	{
		SAFE_DELETE(m_pSceneScript);

		// T�ڽ� ������ ��ĳ����
		m_pSceneScript = new T;

		m_pSceneScript->SetScene(this);

		// T�ڽ� ��ü�� Init�� �ҷ���
		if (!m_pSceneScript->Init())
		{
			SAFE_DELETE(m_pSceneScript);
			return false;
		}

		return true;
	}

public:
	static bool LayerSort(class CLayer* p1, class CLayer* p2);
};

