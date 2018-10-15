#pragma once
#include "SceneScript.h"
class CMainScene :
	public CSceneScript
{
	//CScene에서 CScene을 데이터 멤버로 가지고 있다.
private:
	// CScene friend
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

