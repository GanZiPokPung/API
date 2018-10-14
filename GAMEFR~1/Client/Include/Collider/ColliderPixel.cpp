#include "ColliderPixel.h"
#include "../Core/PathManager.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "ColliderPoint.h"

CColliderPixel::CColliderPixel()
{
}

CColliderPixel::CColliderPixel(const CColliderPixel & pixel)	:
	CCollider(pixel)
{
	m_tInfo = pixel.m_tInfo;
}

CColliderPixel::~CColliderPixel()
{
}

CColliderPixel * CColliderPixel::Create(const string & strTag, const string & strKey, 
	char * pFileName, char r, char g, char b, const string& strPathKey)
{
	CColliderPixel*	pPixel = new CColliderPixel;

	if (!pPixel->Init())
	{
		SAFE_DELETE(pPixel);
		return NULL;
	}

	if(!pPixel->LoadCollisionTexture(strKey, pFileName, r, g, b, strPathKey))
	{ 

		SAFE_DELETE(pPixel);
		return NULL;	
	}

	pPixel->SetTag(strTag);

	return pPixel;
}

bool CColliderPixel::LoadCollisionTexture(const string & strKey, char * pFileName, 
	char r, char g, char b, const string& strPathKey)
{
	m_tInfo.tCollPixel.r = r;
	m_tInfo.tCollPixel.g = g;
	m_tInfo.tCollPixel.b = b;

	FILE*	pFile = NULL;

	string	strPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);
	strPath += pFileName;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (pFile)
	{
		// File Header와 Info header 선언
		BITMAPFILEHEADER	tBf = {};
		BITMAPINFOHEADER	tIf = {};

		fread(&tBf, sizeof(tBf), 1, pFile);
		fread(&tIf, sizeof(tIf), 1, pFile);

		fseek(pFile, tBf.bfOffBits, SEEK_SET);

		m_tInfo.iWidth = tIf.biWidth;
		m_tInfo.iHeight = tIf.biHeight;

		//m_vecPixel.reserve(iWidth * iHeight);
		m_tInfo.vecPixel.resize(tIf.biWidth * tIf.biHeight);

		fread(&m_tInfo.vecPixel[0], sizeof(PIXEL24), tIf.biWidth * tIf.biHeight, pFile);

		PPIXEL24	pLine = new PIXEL24[tIf.biWidth];

		for (int i = 0; i < tIf.biHeight / 2; ++i)
		{
			int	iBottomIdx = tIf.biHeight - (i + 1);
			memcpy(pLine, &m_tInfo.vecPixel[i * tIf.biWidth], sizeof(PIXEL24) * tIf.biWidth);
			memcpy(&m_tInfo.vecPixel[i * tIf.biWidth], &m_tInfo.vecPixel[iBottomIdx * tIf.biWidth],
				sizeof(PIXEL24) * tIf.biWidth);
			memcpy(&m_tInfo.vecPixel[iBottomIdx * tIf.biWidth], pLine, sizeof(PIXEL24) * tIf.biWidth);
		}

		SAFE_DELETE_ARRAY(pLine);

		fclose(pFile);
	}

	return true;
}

void CColliderPixel::Move(const POSITION & tMove)
{
}

void CColliderPixel::Move(float x, float y)
{
}

void CColliderPixel::Move(const POINT & ptMove)
{
}

bool CColliderPixel::Init()
{
	return true;
}

void CColliderPixel::Update(float fTime)
{
}

void CColliderPixel::Render(HDC hDC, float fTime)
{
}

CColliderPixel * CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}

bool CColliderPixel::Collision(CCollider * pCollider)
{
	// 들어온 충돌체의 타입을 판단한다.
	switch (pCollider->GetType())
	{
	case CT_RECT:
		return CollisionPixelToRect(&m_tInfo, ((CColliderRect*)pCollider)->GetRect());
	case CT_SPHERE:
		break;
	case CT_LINE:
		break;
	case CT_POINT:
		break;
	case CT_PIXEL:
		break;
	}

	return false;
}
