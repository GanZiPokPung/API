#include "Texture.h"
#include "../Core/PathManager.h"

CTexture::CTexture()
{
	m_dwColorKey = RGB(255, 0, 255);
}

CTexture::~CTexture()
{
	// �׸��� ������ ������ ������ �ִ� ������ �������ش�.
	SelectObject(m_hMemDC, m_hOldBmp);

	// �ε��� ��Ʈ�� �ڵ��� �����ش�.
	DeleteObject(m_hBmp);

	// �޸� DC�� �����ش�.
	DeleteDC(m_hMemDC);
}

bool CTexture::LoadTexture(HINSTANCE hInst, HDC hDC, TCHAR * pFileName, bool bColorKey,
	COLORREF dwColorKey, const string & strPathKey)
{
	m_dwColorKey = dwColorKey;
	m_bColorKey = bColorKey;
	//Texture Path�� ������ ��
	m_strPath = GET_SINGLE(CPathManager)->FindPathFromWString(strPathKey);
	m_strFileName = pFileName;
	//Texture Path�� ���� �̸��� ��� ������
	m_strFullPath = m_strPath + m_strFileName;

	// �̹����� �ҷ��´�.
	m_hBmp = (HBITMAP)LoadImage(hInst, m_strFullPath.c_str(), IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);

	// �̹��� �ȼ� ������ ���� DC�� �����Ѵ�.
	m_hMemDC = CreateCompatibleDC(hDC);

	// �׸��� ������ �������ش�. �� �Լ��� DC�� �׸��� ������ �����ϰ� ������ ������
	// �ִ� ������ �������ش�.
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBmp);

	// Load�� �̹����� m_hBmp���� m_tInfo�� ������´�.
	GetObject(m_hBmp, sizeof(m_tInfo), &m_tInfo);

	return true;
}
