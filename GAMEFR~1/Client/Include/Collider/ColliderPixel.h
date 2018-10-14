#pragma once
#include "Collider.h"
class CColliderPixel :
	public CCollider
{
private:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& pixel);
	~CColliderPixel();

private:
	PIXELCOLLIDERINFO	m_tInfo;

public:
	PPIXELCOLLIDERINFO GetPixelInfo()
	{
		return &m_tInfo;
	}

public:
	static CColliderPixel* Create(const string& strTag, const string& strKey, char* pFileName,
		char r, char g, char b, const string& strPathKey = TEXTURE_PATH);
	
public:
	bool LoadCollisionTexture(const string& strKey, char* pFileName,
		char r, char g, char b, const string& strPathKey = TEXTURE_PATH);

public:
	virtual void Move(const POSITION& tMove);
	virtual void Move(float x, float y);
	virtual void Move(const POINT& ptMove);

public:
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CColliderPixel* Clone();

public:
	virtual bool Collision(CCollider* pCollider);
};

