#pragma once

#include "../GameFramework.h"

class CMath
{
public:
	static float AngleToRadian(float fAngle);
	static float RadianToAngle(float fRadian);
	static float GetDistance(const POSITION& tPos1, const POSITION& tPos2);
	static float GetAngle(const POSITION& tSrc, const POSITION& tDest);
};

