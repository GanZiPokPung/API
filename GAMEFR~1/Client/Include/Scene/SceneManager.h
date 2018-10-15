#pragma once

#include "../GameFramework.h"

class CSceneManager
{
private:
	//ÇöÀç ¾À
	class CScene*	m_pCurScene;

public:
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	void Render(HDC hDC, float fTime);
	class CScene* CreateScene();

	DECLARE_SINGLE(CSceneManager)
};

