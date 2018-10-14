#include "Math.h"

// 각도 -> 라디안
float CMath::AngleToRadian(float fAngle)
{
	return fAngle * PI / 180.f;
}

// 라디안 -> 각도
float CMath::RadianToAngle(float fRadian)
{
	return fRadian * 180.f / PI;
}

// 거리 구하는 함수 피타고라스 정리
float CMath::GetDistance(const POSITION & tPos1, const POSITION & tPos2)
{
	float	x = tPos1.x - tPos2.x;
	float	y = tPos1.y - tPos2.y;
	return sqrtf(x * x + y * y);
}

float CMath::GetAngle(const POSITION & tSrc, const POSITION & tDest)
{
	// abs : 절대값으로 만들어준다.
	// 밑변
	int	iW = tDest.x - tSrc.x;
	// 빗변
	int	iC = GetDistance(tSrc, tDest);

	// (밑변 / 빗변) = cos(세타)
	// acosf(cos(세타)) = 세타
	float	fRadian = acosf(iW / (float)iC);

	// 각도로 변경
	int	iAngle = RadianToAngle(fRadian);

	// 목적지의 y값이 출발할 y값보다 더 작다면 위에 있으므로
	// 360도 기준으로 바꿔줘야 한다.
	if (tDest.y < tSrc.y)
		iAngle = 360.f - iAngle;

	return iAngle;
}
