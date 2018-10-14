#include "Math.h"

// ���� -> ����
float CMath::AngleToRadian(float fAngle)
{
	return fAngle * PI / 180.f;
}

// ���� -> ����
float CMath::RadianToAngle(float fRadian)
{
	return fRadian * 180.f / PI;
}

// �Ÿ� ���ϴ� �Լ� ��Ÿ��� ����
float CMath::GetDistance(const POSITION & tPos1, const POSITION & tPos2)
{
	float	x = tPos1.x - tPos2.x;
	float	y = tPos1.y - tPos2.y;
	return sqrtf(x * x + y * y);
}

float CMath::GetAngle(const POSITION & tSrc, const POSITION & tDest)
{
	// abs : ���밪���� ������ش�.
	// �غ�
	int	iW = tDest.x - tSrc.x;
	// ����
	int	iC = GetDistance(tSrc, tDest);

	// (�غ� / ����) = cos(��Ÿ)
	// acosf(cos(��Ÿ)) = ��Ÿ
	float	fRadian = acosf(iW / (float)iC);

	// ������ ����
	int	iAngle = RadianToAngle(fRadian);

	// �������� y���� ����� y������ �� �۴ٸ� ���� �����Ƿ�
	// 360�� �������� �ٲ���� �Ѵ�.
	if (tDest.y < tSrc.y)
		iAngle = 360.f - iAngle;

	return iAngle;
}
