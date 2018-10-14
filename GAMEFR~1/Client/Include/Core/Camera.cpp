#include "Camera.h"
#include "Input.h"
#include "../Object/Obj.h"
#include "../Core.h"

DEFINITION_SINGLE(CCamera)

CCamera::CCamera()	:
	m_pTarget(NULL)
{
}

CCamera::~CCamera()
{
	SAFE_RELEASE(m_pTarget);
}

void CCamera::SetResolution(float x, float y)
{
	// 해상도 설정
	m_tResolution.x = x;
	m_tResolution.y = y;
}

void CCamera::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CCamera::SetTarget(CObj * pTarget)
{
	// 기존에 따라다니는 타겟이 있었다면 타겟을 Release 해줌
	SAFE_RELEASE(m_pTarget);

	// 새로운 타겟을 대입
	// (refCnt 증가)
	m_pTarget = pTarget;
	if(m_pTarget)
		m_pTarget->AddRef();
}

bool CCamera::Init(float x, float y, float cx, float cy)
{
	// 위치, 맵? 해상도, 속도, 피봇값(카메라가 보여질 위치에서의 비율)
	m_tPos.x = x;
	m_tPos.y = y;

	m_tResolution.x = cx;
	m_tResolution.y = cy;

	m_fSpeed = 1000.f;

	m_tTargetPivot.x = 0.7f;
	m_tTargetPivot.y = 0.7f;

	return true;
}

// 자유시점 모드
void CCamera::Input(float fTime)
{
	// 따라 다니는 타겟이 있다면 빠져나옴
	if (m_pTarget)
		return;

	// 키 입력에 따라서 카메라가 이동한다.

	float	fSpeed = m_fSpeed * fTime;

	// 카메라가 위로 이동
	if (KEYDOWN("MoveUp") || KEYPUSH("MoveUp"))
	{
		m_tPos.y -= fSpeed;

		//0보다 작아지면 0 고정 
		if (m_tPos.y <= 0.f)
			m_tPos.y = 0.f;
	}

	// 아래
	if (KEYDOWN("MoveDown") || KEYPUSH("MoveDown"))
	{
		m_tPos.y += fSpeed;

		//밑 영역에 카메라가 못 나가도록
		if (m_tPos.y >= m_tResolution.y - RESOLUTION_H)
			m_tPos.y = m_tResolution.y - RESOLUTION_H;
	}

	// 왼쪽
	if (KEYDOWN("MoveLeft") || KEYPUSH("MoveLeft"))
	{
		m_tPos.x -= fSpeed;

		if (m_tPos.x <= 0.f)
			m_tPos.x = 0.f;
	}

	// 오른쪽
	if (KEYDOWN("MoveRight") || KEYPUSH("MoveRight"))
	{
		m_tPos.x += fSpeed;

		if (m_tPos.x >= m_tResolution.x - RESOLUTION_W)
			m_tPos.x = m_tResolution.x - RESOLUTION_W;
	}
}

void CCamera::Update(float fTime)
{
	// 타겟의 위치에 맞는 카메라 위치 조정
	// 타겟을 따라 다닌다.
	if (m_pTarget)
	{
		POSITION	tPos = m_pTarget->GetPos();

		float	fRight = m_tResolution.x - RESOLUTION_W * (1.f - m_tTargetPivot.x);
		float	fBottom = m_tResolution.y - RESOLUTION_H * (1.f - m_tTargetPivot.y);

		// 좌 우 검사
		if (tPos.x < RESOLUTION_W * m_tTargetPivot.x)
			// 타겟이 해상도 범위 안에 있을때
			m_tPos.x = 0.f;										// 카메라 움직임 없음
 		else if (tPos.x > fRight)
			// 타겟이 해상도 범위(오른쪽)를 벗어났을때
			m_tPos.x = m_tResolution.x - RESOLUTION_W;			// 맵 해상도 - 윈도우 해상도
		else
			// 위 두 상황이 아니라면 캐릭터를 따라다님(피봇값에 따라)
			m_tPos.x = tPos.x - RESOLUTION_W * m_tTargetPivot.x;  

		// 위 아래 검사
		// 위와 동일
		if (tPos.y < RESOLUTION_H * m_tTargetPivot.y)
			m_tPos.y = 0.f;

		else if (tPos.y > fBottom)
			m_tPos.y = m_tResolution.y - RESOLUTION_H;

		else
			m_tPos.y = tPos.y - RESOLUTION_H * m_tTargetPivot.y;

	}
}
