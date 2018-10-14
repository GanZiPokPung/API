#pragma once

#include "../GameFramework.h"

class CSceneScript
{
protected:
	friend class CScene;

protected:
	CSceneScript();
	virtual ~CSceneScript() = 0;

protected:
	class CScene*	m_pScene;

public:
	void SetScene(class CScene* pScene);

public:
	virtual bool Init() = 0;
	virtual void Input(float fTime) = 0;
	virtual void Update(float fTime) = 0;
	virtual void Render(HDC hDC, float fTime) = 0;
};

