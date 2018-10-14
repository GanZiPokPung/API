#pragma once
#include "../Base.h"
class CTexture :
	public CBase
{
private:
	friend class CResourcesManager;

private:
	CTexture();
	~CTexture();

private:
	wstring		m_strPath;
	wstring		m_strFileName;
	wstring		m_strFullPath;

private:
	HDC			m_hMemDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hOldBmp;
	BITMAP		m_tInfo;
	bool		m_bColorKey;
	COLORREF	m_dwColorKey;

public:
	UINT GetWidth()	const
	{
		return m_tInfo.bmWidth;
	}

	UINT GetHeight()	const
	{
		return m_tInfo.bmHeight;
	}

	HDC GetMemDC()	const
	{
		return m_hMemDC;
	}
	
	bool GetColorKeyEnable()	const
	{
		return m_bColorKey;
	}

	COLORREF GetColorKey()	const
	{
		return m_dwColorKey;
	}

	void* GetPixelData()	const
	{
		return m_tInfo.bmBits;
	}

public:
	bool LoadTexture(HINSTANCE hInst, HDC hDC, TCHAR* pFileName, bool bColorKey = false,
		COLORREF dwColorKey = RGB(255, 0, 255), const string& strPathKey = TEXTURE_PATH);
};

