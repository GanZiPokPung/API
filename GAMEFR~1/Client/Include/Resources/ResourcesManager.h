#pragma once

#include "../GameFramework.h"

class CResourcesManager
{
private:
	HINSTANCE	m_hInst;
	HDC			m_hDC;

private:
	unordered_map<string, class CTexture*>	m_mapTexture;

public:
	bool Init(HINSTANCE hInst, HDC hDC);
	class CTexture* LoadTexture(const string& strKey, TCHAR* pFileName,
		bool bColorKey = false, COLORREF dwColorKey = RGB(255, 0, 255),
		const string& strPathKey = TEXTURE_PATH);
	class CTexture* FindTexture(const string& strKey);

	DECLARE_SINGLE(CResourcesManager)
};

