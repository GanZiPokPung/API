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
	// 1�ʿ� ó�� ������ tick�� ���´�.
	// Ŭ���� ������ ���� �ʿ���(�̰� �̿��Ͽ� �����⸦ ���� �ð����� ���ϱ� ����)
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
	// ������Ʈ ���۽�
	// �޼��� ������ ��꿡 �ʿ��� m_prev�� ���� ä���ֱ� ����
	if (!m_bStartUpdate)
	{
		m_bStartUpdate = true;
		//���� ������ Ŭ����
		QueryPerformanceCounter(&m_tPrev);
		return;
	}

	// ���� ������ Ŭ����
	LARGE_INTEGER	tTime;
	QueryPerformanceCounter(&tTime);

	// ���� �ð��� ���� Ŭ������ term�� ���Ͽ� �ʴ� �������� ������ �ð��ʰ��� ���Ѵ�.
	// second �� ����
	m_fDeltaTime = (tTime.QuadPart - m_tPrev.QuadPart) / (float)m_tSecond.QuadPart;

	// �ð����� ���� �Ǿ����Ƿ� m_prev�� ���� Ŭ������ �����Ѵ�.
	m_tPrev = tTime;

	// FPSTime�� deltaTime ��ŭ ����ؼ� �����ȴ�.
	m_fFPSTime += m_fDeltaTime;
	++m_iFrame;

	// ���� �����ӱ��� �Դٸ�
	if (m_iFrame == m_iFrameLimit)
	{
		// ���� ���� �ð��� m_frame�� ������ FPS�� ���Ѵ�
		// m_FPSTime�� 1�̶�� 60, 2��� 30,.....
		// �׸��� frame�� FPSTime�� �ʱ�ȭ �Ѵ�.
		m_fFPS = m_iFrame / m_fFPSTime;
		m_iFrame = 0;
		m_fFPSTime = 0.f;

		// ȭ��󿡼� fps�� ����ϱ� ���� ���ڿ��� ���� �����Ѵ�.
		char	str[128] = {};

		sprintf_s(str, "DeltaTime : %f FPS : %f", m_fDeltaTime, m_fFPS);
		SetWindowTextA(WINDOWHANDLE, str);
	}
}

void CTimer::Destroy()
{
	delete	this;
}
