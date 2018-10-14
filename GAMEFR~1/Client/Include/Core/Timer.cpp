#include "Timer.h"
#include "../Core.h"

CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

bool CTimer::Init()
{
	// 1초에 처리 가능한 tick을 얻어온다.
	// 클럭수 지정을 위해 필요함(이걸 이용하여 나누기를 통해 시간값을 구하기 위해)
	QueryPerformanceFrequency(&m_tSecond);

	m_fDeltaTime = 0.f;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFrame = 0;
	m_iFrameLimit = 60;
	m_bStartUpdate = false;

	return true;
}

void CTimer::Update()
{
	// 업데이트 시작시
	// 메서드 내에서 계산에 필요한 m_prev에 값을 채워넣기 위함
	if (!m_bStartUpdate)
	{
		m_bStartUpdate = true;
		//이전 시점의 클럭수
		QueryPerformanceCounter(&m_tPrev);
		return;
	}

	// 현재 시점의 클럭수
	LARGE_INTEGER	tTime;
	QueryPerformanceCounter(&tTime);

	// 이전 시간과 현재 클럭수의 term을 구하여 초당 진동수와 나누어 시간초값을 구한다.
	// second 초 기준
	m_fDeltaTime = (tTime.QuadPart - m_tPrev.QuadPart) / (float)m_tSecond.QuadPart;

	// 시간값이 갱신 되었으므로 m_prev를 현재 클럭수로 갱신한다.
	m_tPrev = tTime;

	// FPSTime은 deltaTime 만큼 계속해서 누적된다.
	m_fFPSTime += m_fDeltaTime;
	++m_iFrame;

	// 제한 프레임까지 왔다면
	if (m_iFrame == m_iFrameLimit)
	{
		// 현재 누적 시간과 m_frame을 나누어 FPS를 구한다
		// m_FPSTime이 1이라면 60, 2라면 30,.....
		// 그리고 frame과 FPSTime을 초기화 한다.
		m_fFPS = m_iFrame / m_fFPSTime;
		m_iFrame = 0;
		m_fFPSTime = 0.f;

		// 화면상에서 fps를 출력하기 위해 문자열로 값을 저장한다.
		char	str[128] = {};

		sprintf_s(str, "DeltaTime : %f FPS : %f", m_fDeltaTime, m_fFPS);
		SetWindowTextA(WINDOWHANDLE, str);
	}
}

void CTimer::Destroy()
{
	delete	this;
}
