#pragma once

#include "../Base.h"

class CObj :
	public CBase
{
protected:
	friend class CObjectManager;
	friend class CCollisionManager;

protected:
	CObj();
	CObj(const CObj& obj);
	virtual ~CObj() = 0;

protected:
	class CLayer*	m_pLayer;
	class CScene*	m_pScene;
	string			m_strTag;
	list<class CCollider*>	m_ColliderList;
	bool			m_bEnable;
	bool			m_bAlive;
	bool			m_bUseGravity;
	float			m_fGravityTime;
	bool			m_bJump;
	float			m_fJumpAccel;

protected:
	class CTexture*	m_pTexture;
	class CAnimation*	m_pAnimation;

protected:
	POSITION		m_tPos;
	_SIZE			m_tSize;
	POSITION		m_tPivot;
	bool			m_bCamera;

public:
	class CTexture* GetTexture()	const;
	class CAnimation* GetAnimation()	const;

	bool GetCameraEnable()	const
	{
		return m_bCamera;
	}

	bool GetEnable()	const
	{
		return m_bEnable;
	}

	bool GetAlive()	const
	{
		return m_bAlive;
	}

	POSITION GetPos()	const
	{
		return m_tPos;
	}

	_SIZE GetSize()	const
	{
		return m_tSize;
	}

	POSITION GetPivot()	const
	{
		return m_tPivot;
	}

public:
	void SetPos(float x, float y)
	{
		m_tPos = POSITION(x, y);
	}

	void SetPos(const POSITION& tPos)
	{
		m_tPos = tPos;
	}

	void SetPos(float fPos[2])
	{
		m_tPos = fPos;
	}

	void SetSize(float x, float y)
	{
		m_tSize = _SIZE(x, y);
	}

	void SetSize(const _SIZE& tSize)
	{
		m_tSize = tSize;
	}

	void SetSizeFromImageSize();
	void SetSizeFromAnimationFrameSize();

	void SetPivot(float x, float y)
	{
		if (x > 1.f)
			x = 1.f;
		else if (x < 0.f)
			x = 0.f;

		if (y > 1.f)
			y = 1.f;
		else if (y < 0.f)
			y = 0.f;

		m_tPivot = POSITION(x, y);
	}

	void SetPivot(const _tagPosition& tPivot)
	{
		m_tPivot = tPivot;

		if (m_tPivot.x > 1.f)
			m_tPivot.x = 1.f;
		else if (m_tPivot.x < 0.f)
			m_tPivot.x = 0.f;

		if (m_tPivot.y > 1.f)
			m_tPivot.y = 1.f;
		else if (m_tPivot.y < 0.f)
			m_tPivot.y = 0.f;
	}

	void SetAnimationFrameSize();

	void SetEnable(bool bEnable)
	{
		m_bEnable = bEnable;
	}

	void DisableCamera()
	{
		m_bCamera = false;
	}

	void Die()
	{
		m_bAlive = false;
	}

	void UseGravity(bool bGravity = true)
	{
		m_bUseGravity = bGravity;
	}

	void InitGravityTime()
	{
		m_fGravityTime = 0.f;
	}

public:
	class CLayer* GetLayer()	const
	{
		return m_pLayer;
	}

	class CScene* GetScene()	const
	{
		return m_pScene;
	}

	string GetTag()	const
	{
		return m_strTag;
	}

public:
	void SetScene(class CScene* pScene);
	void SetLayer(class CLayer* pLayer);

	void SetTag(const string& strTag)
	{
		m_strTag = strTag;
	}

public:
	void SetTexture(class CTexture* pTexture);
	void SetTexture(const string& strKey, TCHAR* pFileName = NULL,
		bool bColorKey = false, COLORREF dwColorKey = RGB(255, 0, 255),
		const string& strPathKey = TEXTURE_PATH);

	void SetAnimation(class CAnimation* pAnimation);
	void SetAnimation(const char* pName, ANIMATION_TYPE eType,
		ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight,
		UINT iFrameMaxX, UINT iFrameMaxY, float fLimitTime);
	void SetAnimation(const char* pFileName, 
		const string& strPathKey = ANIMATION_PATH);

public:
	void AddCollider(class CCollider* pCollider);
	bool CheckCollider();
	class CCollider* FindCollider(const string& strTag);

public:
	virtual bool Init() = 0;
	virtual void Input(float fTime) = 0;
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual CObj* Clone() = 0;
};

