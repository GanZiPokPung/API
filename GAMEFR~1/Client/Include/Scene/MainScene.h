#pragma once
#include "SceneScript.h"
class CMainScene :
	public CSceneScript
{
private:
	friend class CScene;

private:
	CMainScene();
	~CMainScene();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};

