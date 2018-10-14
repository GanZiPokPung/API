#include "Animation.h"
#include "../Core/PathManager.h"


CAnimation::CAnimation()	:
	m_iCurrentAnimation(0),
	m_bFrameEnd(false)
{
}


CAnimation::~CAnimation()
{
}

CAnimation * CAnimation::Create(const char * pName, ANIMATION_TYPE eType, 
	ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight, UINT iFrameMaxX,
	UINT iFrameMaxY, float fLimitTime)
{
	CAnimation*	pAnimation = new CAnimation;

	if (!pAnimation->Init(pName, eType, eOption, iWidth, iHeight, iFrameMaxX,
		iFrameMaxY, fLimitTime))
	{
		delete	pAnimation;
		return NULL;
	}

	return pAnimation;
}

CAnimation * CAnimation::Create(const char * pFileName,
	const string& strPathKey)
{
	CAnimation*	pAnimation = new CAnimation;

	if (!pAnimation->Load(pFileName, strPathKey))
	{
		SAFE_DELETE(pAnimation);
		return NULL;
	}

	return pAnimation;
}

bool CAnimation::Init(const char * pName, ANIMATION_TYPE eType, 
	ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight, UINT iFrameMaxX, 
	UINT iFrameMaxY, float fLimitTime)
{
	m_tInfo.strName = pName;
	m_tInfo.eType = eType;
	m_tInfo.eOption = eOption;
	m_tInfo.iImageWidth = iWidth;
	m_tInfo.iImageHeight = iHeight;
	m_tInfo.iFrameMaxX = iFrameMaxX;
	m_tInfo.iFrameMaxY = iFrameMaxY;
	m_tInfo.iFrameX = 0;
	m_tInfo.iFrameY = 0;

	m_tInfo.fLimitTime = fLimitTime;
	m_tInfo.fTime = 0.f;

	return true;
}

bool CAnimation::Init(const char * pFileName, const string & strPathKey)
{
	return true;
}

void CAnimation::Update(float fTime)
{
	if (m_tInfo.eOption == AO_LOOP)
	{
		if (m_bFrameEnd)
			m_bFrameEnd = false;
	}

	if (m_bFrameEnd)
		return;

	m_tInfo.fTime += fTime;

	UINT	iFrameMax = m_vecLineFrameCount[m_iCurrentAnimation];

	if (m_tInfo.eType == AT_ALL)
	{
		iFrameMax = 0;
		for (size_t i = 0; i < m_vecLineFrameCount.size(); ++i)
		{
			iFrameMax += m_vecLineFrameCount[i];
		}
	}

	float	fChangeTime = m_tInfo.fLimitTime / iFrameMax;

	while (m_tInfo.fTime >= fChangeTime)
	{
		++m_tInfo.iFrameX;
		m_tInfo.fTime -= fChangeTime;

		if (m_tInfo.iFrameX == m_vecLineFrameCount[m_tInfo.iFrameY])
		{
			m_tInfo.iFrameX = 0;

			if (m_tInfo.eType == AT_ALL)
			{
				++m_tInfo.iFrameY;

				if (m_tInfo.iFrameY == m_tInfo.iFrameMaxY)
				{
					m_tInfo.iFrameY = 0;
					m_bFrameEnd = true;
				}
			}

			else
				m_bFrameEnd = true;
		}
	}
}

CAnimation * CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::ChangeAnimation(UINT iAnimation)
{
	if (m_iCurrentAnimation == iAnimation)
		return;

	else if (iAnimation >= m_vecLineFrameCount.size())
		return;

	m_tInfo.iFrameX = 0;

	m_tInfo.iFrameY = iAnimation;
	m_iCurrentAnimation = iAnimation;
}

bool CAnimation::Load(const char * pFileName, const string& strPathKey)
{
	string	strPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);
	strPath += pFileName;

	FILE*	pFile = NULL;

	fopen_s(&pFile, strPath.c_str(), "rb");

	m_tInfo.iFrameX = 0;
	m_tInfo.iFrameY = 0;
	m_tInfo.fTime = 0.f;
	m_iCurrentAnimation = 0;

	if (pFile)
	{
		// 이름을 읽어온다.
		int	iLength = 0;
		fread(&iLength, 4, 1, pFile);

		char*	pName = new char[iLength + 1];
		memset(pName, 0, iLength + 1);

		// 문자열을 읽어온다.
		fread(pName, 1, iLength, pFile);

		m_tInfo.strName = pName;
		SAFE_DELETE_ARRAY(pName);

		// 타입 읽어온다
		fread(&m_tInfo.eType, 4, 1, pFile);

		// 옵션 읽어온다
		fread(&m_tInfo.eOption, 4, 1, pFile);

		// Frame x, y 저장
		fread(&m_tInfo.iFrameMaxX, 4, 1, pFile);
		fread(&m_tInfo.iFrameMaxY, 4, 1, pFile);

		// fLimitTime 저장
		fread(&m_tInfo.fLimitTime, 4, 1, pFile);

		m_vecLineFrameCount.clear();

		for (int i = 0; i < m_tInfo.iFrameMaxY; ++i)
		{
			UINT	iFrameCount;
			fread(&iFrameCount, 4, 1, pFile);
			m_vecLineFrameCount.push_back(iFrameCount);
		}

		fclose(pFile);

		return true;
	}

	return false;
}
