
#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <stdlib.h>
#include <typeinfo.h>
#include <algorithm>
#include <conio.h>
#include <functional>

using namespace std;

// 사용자정의 헤더파일
#include "Macro.h"

// TransparentBlt를 사용하기 위한 라이브러리를 추가해준다.
#pragma comment(lib, "msimg32")

// 경로 키
#define	ROOT_PATH		"RootPath"
#define	TEXTURE_PATH	"TexturePath"
#define	ANIMATION_PATH	"AnimationPath"


#define	PI		3.141592f
#define	GRAVITY	0.98f

template <typename T>
void Safe_Delete_VecList(T& con)
{
	T::iterator	iter;
	T::iterator	iterEnd = con.end();

	for (iter = con.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Destroy();
	}

	con.clear();
}

template <typename T>
void Safe_Delete_Map(T& con)
{
	T::iterator	iter;
	T::iterator	iterEnd = con.end();

	for (iter = con.begin(); iter != iterEnd; ++iter)
	{
		iter->second->Destroy();
	}

	con.clear();
}

template <typename T>
void Safe_Release_VecList(T& con)
{
	T::iterator	iter;
	T::iterator	iterEnd = con.end();

	for (iter = con.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Release();
	}

	con.clear();
}

template <typename T>
void Safe_Release_Map(T& con)
{
	T::iterator	iter;
	T::iterator	iterEnd = con.end();

	for (iter = con.begin(); iter != iterEnd; ++iter)
	{
		iter->second->Release();
	}

	con.clear();
}

// 위치 정보 구조체
typedef struct _tagPosition
{
	float	x, y;

	_tagPosition() :
		x(0.f),
		y(0.f)
	{
	}

	_tagPosition(const _tagPosition& pos)
	{
		*this = pos;
	}

	_tagPosition(const POINT& pos)
	{
		*this = pos;
	}

	_tagPosition(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}

	void operator =(const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	void operator =(const POINT& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	void operator =(float fArr[2])
	{
		x = fArr[0];
		y = fArr[1];
	}

	void operator =(int iArr[2])
	{
		x = iArr[0];
		y = iArr[1];
	}

	_tagPosition operator +(const _tagPosition& pos)
	{
		_tagPosition	tResult;

		tResult.x = x + pos.x;
		tResult.y = y + pos.y;

		return tResult;
	}

	_tagPosition operator +(const POINT& pos)
	{
		_tagPosition	tResult;

		tResult.x = x + pos.x;
		tResult.y = y + pos.y;

		return tResult;
	}

	_tagPosition operator +(float f[2])
	{
		_tagPosition	tResult;

		tResult.x = x + f[0];
		tResult.y = y + f[1];

		return tResult;
	}

	void operator +=(const _tagPosition& pos)
	{
		x += pos.x;
		y += pos.y;
	}

	void operator +=(const POINT& pt)
	{
		x += pt.x;
		y += pt.y;
	}

	_tagPosition operator -(const _tagPosition& pos)
	{
		_tagPosition	tResult;

		tResult.x = x - pos.x;
		tResult.y = y - pos.y;

		return tResult;
	}

	_tagPosition operator -(const POINT& pos)
	{
		_tagPosition	tResult;

		tResult.x = x - pos.x;
		tResult.y = y - pos.y;

		return tResult;
	}

	_tagPosition operator -(float f[2])
	{
		_tagPosition	tResult;

		tResult.x = x - f[0];
		tResult.y = y - f[1];

		return tResult;
	}

	void operator -=(const _tagPosition& pos)
	{
		x -= pos.x;
		y -= pos.y;
	}

	_tagPosition operator *(const _tagPosition& pos)
	{
		_tagPosition	tResult;

		tResult.x = x * pos.x;
		tResult.y = y * pos.y;

		return tResult;
	}

	_tagPosition operator *(float f[2])
	{
		_tagPosition	tResult;

		tResult.x = x * f[0];
		tResult.y = y * f[1];

		return tResult;
	}

	void operator *=(const _tagPosition& pos)
	{
		x *= pos.x;
		y *= pos.y;
	}

	bool operator ==(const _tagPosition& pos)
	{
		return x == pos.x && y == pos.y;
	}

	bool operator ==(const POINT& pos)
	{
		return x == pos.x && y == pos.y;
	}

	bool operator !=(const _tagPosition& pos)
	{
		return x != pos.x || y != pos.y;
	}

	bool operator !=(const POINT& pos)
	{
		return x != pos.x || y != pos.y;
	}
}POSITION, *PPOSITION, _SIZE, *_PSIZE;

typedef struct _tagRectangle
{
	float	fLeft;
	float	fTop;
	float	fRight;
	float	fBottom;

	_tagRectangle() :
		fLeft(0.f),
		fTop(0.f),
		fRight(0.f),
		fBottom(0.f)
	{
	}

	_tagRectangle(float fL, float fT, float fR, float fB) :
		fLeft(fL),
		fTop(fT),
		fRight(fR),
		fBottom(fB)
	{
	}

	_tagRectangle(const _tagRectangle& rc)
	{
		*this = rc;
	}

	void operator =(const _tagRectangle& rc)
	{
		fLeft = rc.fLeft;
		fTop = rc.fTop;
		fRight = rc.fRight;
		fBottom = rc.fBottom;
	}

	_tagRectangle operator +(const _tagRectangle& rc)
	{
		_tagRectangle	tRC;

		tRC.fLeft = fLeft + rc.fLeft;
		tRC.fTop = fTop + rc.fTop;
		tRC.fRight = fRight + rc.fRight;
		tRC.fBottom = fBottom + rc.fBottom;

		return tRC;
	}

	_tagRectangle operator +(const RECT& rc)
	{
		_tagRectangle	tRC;

		tRC.fLeft = fLeft + rc.left;
		tRC.fTop = fTop + rc.top;
		tRC.fRight = fRight + rc.right;
		tRC.fBottom = fBottom + rc.bottom;

		return tRC;
	}

	_tagRectangle operator +(const POSITION& pt)
	{
		_tagRectangle	tRC;

		tRC.fLeft = fLeft + pt.x;
		tRC.fTop = fTop + pt.y;
		tRC.fRight = fRight + pt.x;
		tRC.fBottom = fBottom + pt.y;

		return tRC;
	}

	_tagRectangle operator +(const POINT& pt)
	{
		_tagRectangle	tRC;

		tRC.fLeft = fLeft + pt.x;
		tRC.fTop = fTop + pt.y;
		tRC.fRight = fRight + pt.x;
		tRC.fBottom = fBottom + pt.y;

		return tRC;
	}

	// -
	_tagRectangle operator -(const _tagRectangle& rc)
	{
		_tagRectangle	tRC;

		tRC.fLeft = fLeft - rc.fLeft;
		tRC.fTop = fTop - rc.fTop;
		tRC.fRight = fRight - rc.fRight;
		tRC.fBottom = fBottom - rc.fBottom;

		return tRC;
	}

	_tagRectangle operator -(const RECT& rc)
	{
		_tagRectangle	tRC;

		tRC.fLeft = fLeft - rc.left;
		tRC.fTop = fTop - rc.top;
		tRC.fRight = fRight - rc.right;
		tRC.fBottom = fBottom - rc.bottom;

		return tRC;
	}

	_tagRectangle operator -(const POSITION& pt)
	{
		_tagRectangle	tRC;

		tRC.fLeft = fLeft - pt.x;
		tRC.fTop = fTop - pt.y;
		tRC.fRight = fRight - pt.x;
		tRC.fBottom = fBottom - pt.y;

		return tRC;
	}

	_tagRectangle operator -(const POINT& pt)
	{
		_tagRectangle	tRC;

		tRC.fLeft = fLeft - pt.x;
		tRC.fTop = fTop - pt.y;
		tRC.fRight = fRight - pt.x;
		tRC.fBottom = fBottom - pt.y;

		return tRC;
	}

	void operator -=(const POSITION& pt)
	{
		fLeft = fLeft - pt.x;
		fTop = fTop - pt.y;
		fRight = fRight - pt.x;
		fBottom = fBottom - pt.y;
	}
}RECTANGLE, *PRECTANGLE;

// Sphere
typedef struct _tagSphere
{
	POSITION	tCenter;
	float		fRadius;

	_tagSphere() :
		tCenter(0.f, 0.f),
		fRadius(0.f)
	{
	}

	_tagSphere(const _tagSphere& rhs)
	{
		*this = rhs;
	}

	_tagSphere(float x, float y, float r) :
		tCenter(x, y),
		fRadius(r)
	{
	}

	_tagSphere(const POSITION& tC, float r) :
		tCenter(tC),
		fRadius(r)
	{
	}

	_tagSphere(const POINT& tC, float r) :
		tCenter(tC.x, tC.y),
		fRadius(r)
	{
	}

	void operator =(const _tagSphere& tSphere)
	{
		tCenter = tSphere.tCenter;
		fRadius = tSphere.fRadius;
	}

	_tagSphere operator +(const _tagSphere& tSphere)
	{
		_tagSphere	tSp;

		tSp.tCenter = tCenter + tSphere.tCenter;
		tSp.fRadius = fRadius + tSphere.fRadius;

		return tSp;
	}

	_tagSphere operator +(const POSITION& tC)
	{
		_tagSphere	tSphere;

		tSphere.tCenter = tCenter + tC;
		tSphere.fRadius = fRadius;

		return tSphere;
	}

	_tagSphere operator +(const POINT& pt)
	{
		_tagSphere	tSphere;

		tSphere.tCenter = tCenter + pt;
		tSphere.fRadius = fRadius;

		return tSphere;
	}

	void operator +=(const _tagSphere& tSphere)
	{
		tCenter += tSphere.tCenter;
		fRadius += tSphere.fRadius;
	}

	void operator +=(const POSITION& tPos)
	{
		tCenter += tPos;
	}

	void operator +=(const POINT& pt)
	{
		tCenter += pt;
	}

	// -
	_tagSphere operator -(const _tagSphere& tSphere)
	{
		_tagSphere	tSp;

		tSp.tCenter = tCenter - tSphere.tCenter;
		tSp.fRadius = fRadius - tSphere.fRadius;

		return tSp;
	}

	_tagSphere operator -(const POSITION& tPos)
	{
		_tagSphere	tSphere;

		tSphere.tCenter = tCenter - tPos;
		tSphere.fRadius = fRadius;

		return tSphere;
	}

	_tagSphere operator -(const POINT& pt)
	{
		_tagSphere	tSphere;

		tSphere.tCenter = tCenter - pt;
		tSphere.fRadius = fRadius;

		return tSphere;
	}

	void operator -=(const POSITION& pt)
	{
		tCenter -= pt;
	}
}SPHERE, *PSPHERE;

// 24Bit Pixel
typedef struct _tagPixel24
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}PIXEL24, *PPIXEL24;

typedef struct _tagPixelColliderInfo
{
	vector<PIXEL24>		vecPixel;
	UINT		iWidth;
	UINT		iHeight;
	PIXEL24		tCollPixel;
	POSITION	tIntersectPos;
}PIXELCOLLIDERINFO, *PPIXELCOLLIDERINFO;

enum ANIMATION_TYPE
{
	AT_LINE,
	AT_ALL
};

enum ANIMATION_OPTION
{
	AO_ONCE,
	AO_LOOP
};

// Collider Type
enum COLLIDER_TYPE
{
	CT_RECT,
	CT_SPHERE,
	CT_LINE,
	CT_PIXEL,
	CT_POINT
};

// Button State
enum BUTTON_STATE
{
	BS_NONE,
	BS_MOUSEON,
	BS_CLICK
};

//enum CHECKBUTTON_STATE
//{
//	CS_CHECKED,
//	CS_UNCHECKED
//};