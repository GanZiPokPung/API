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
	// Animation 인스턴스를 동적할당하고 초기화하여 반환함.
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
	// Animation 인스턴스를 동적할당하고 로드하여 반환함.
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
	// 만약 애니메이션이 루프를 돈다면
	if (m_tInfo.eOption == AO_LOOP)
	{
		if (m_bFrameEnd)
			m_bFrameEnd = false;
	}

	// Frame의 끝일때 더이상 update하면 안되므로 리턴
	if (m_bFrameEnd)
		return;

	// deltaTime만큼 time값 누적  
	m_tInfo.fTime += fTime;

	// vector의 인덱스 접근을 통해 Object의 현재 Anim의 최대 Frame수를 가지고 옴
	// 라인 형식일때
	UINT	iFrameMax = m_vecLineFrameCount[m_iCurrentAnimation];

	// animation의 texture가 한 파일에 모두 있을때
	// Atlas
	if (m_tInfo.eType == AT_ALL)
	{
		iFrameMax = 0;
		for (size_t i = 0; i < m_vecLineFrameCount.size(); ++i)
		{
			iFrameMax += m_vecLineFrameCount[i];
		}
	}

	// 한 프레임당 시간값 
	float	fChangeTime = m_tInfo.fLimitTime / iFrameMax;

	// 프레임 계산 
	while (m_tInfo.fTime >= fChangeTime)
	{
		++m_tInfo.iFrameX;
		m_tInfo.fTime -= fChangeTime;

		if (m_tInfo.iFrameX == m_vecLineFrameCount[m_tInfo.iFrameY])
		{
			m_tInfo.iFrameX = 0;


			// 파일이 한번에 있는게 아닌 분할시에
			if (m_tInfo.eType == AT_ALL)
			{
				// frameY를 증가하고 끝까지 가면 0으로 돌아감
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
	// 현재 애니메이션을 바꾼다.
	
	// 애니메이션 인덱스가 vector size를 넘어가거나 바꾸려는 애니메이션이 
	// 현재 돌아가는 애니메이션과 똑같다면 return
	if (m_iCurrentAnimation == iAnimation)
		return;

	else if (iAnimation >= m_vecLineFrameCount.size())
		return;

	m_tInfo.iFrameX = 0;

	// FrameY가 실질적으로 Animation Motion이다.
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
		// 전체 Anim Time
		fread(&m_tInfo.fLimitTime, 4, 1, pFile);

		// animation별 frame cnt가 몇개 있는지 불러와서
		// clear한 vecLineFrameCount 컨케이너에 push_back 한다.
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
