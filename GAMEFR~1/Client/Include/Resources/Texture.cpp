#include "Texture.h"
#include "../Core/PathManager.h"

CTexture::CTexture()
{
	m_dwColorKey = RGB(255, 0, 255);
}

CTexture::~CTexture()
{
	// 그리기 도구를 기존에 가지고 있던 도구로 변경해준다.
	SelectObject(m_hMemDC, m_hOldBmp);

	// 로드한 비트맵 핸들을 지워준다.
	DeleteObject(m_hBmp);

	// 메모리 DC를 지워준다.
	DeleteDC(m_hMemDC);
}

bool CTexture::LoadTexture(HINSTANCE hInst, HDC hDC, TCHAR * pFileName, bool bColorKey,
	COLORREF dwColorKey, const string & strPathKey)
{
	m_dwColorKey = dwColorKey;
	m_bColorKey = bColorKey;
	//Texture Path를 가지고 옴
	m_strPath = GET_SINGLE(CPathManager)->FindPathFromWString(strPathKey);
	m_strFileName = pFileName;
	//Texture Path와 파일 이름과 축약 연산함
	m_strFullPath = m_strPath + m_strFileName;

	// 이미지를 불러온다.
	m_hBmp = (HBITMAP)LoadImage(hInst, m_strFullPath.c_str(), IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);

	// 이미지 픽셀 정보를 담을 DC를 생성한다.
	m_hMemDC = CreateCompatibleDC(hDC);

	// 그리기 도구를 변경해준다. 이 함수는 DC의 그리기 도구를 변경하고 기존에 가지고
	// 있던 도구를 리턴해준다.
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBmp);

	// Load한 이미지를 m_hBmp에서 m_tInfo로 가지고온다.
	GetObject(m_hBmp, sizeof(m_tInfo), &m_tInfo);

	return true;
}
