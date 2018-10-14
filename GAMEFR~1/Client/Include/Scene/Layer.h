#pragma once

#include "../GameFramework.h"

class CLayer
{
private:
	friend class CScene;

private:
	CLayer();
	~CLayer();

private:
	string	m_strName;
	int		m_iZOrder;
	class CScene*		m_pScene;
	list<class CObj*>	m_ObjList;

public:
	int GetZOrder()	const
	{
		return m_iZOrder;
	}

	string GetName()	const
	{
		return m_strName;
	}

	class CScene* GetScene()	const
	{
		return m_pScene;
	}

public:
	void SetZOrder(int iZOrder)
	{
		m_iZOrder = iZOrder;
	}

	void SetName(const string& strName)
	{
		m_strName = strName;
	}

	void SetScene(CScene* pScene)
	{
		m_pScene = pScene;
	}

public:
	bool Init(const string& strName, int iZOrder);
	void Input(float fTime);
	void Update(float fTime);
	void Render(HDC hDC, float fTime);
	void Destroy();

public:
	void AddObject(class CObj* pObj);
	class CObj* FindObjectFromTag(const string& strTag);
};

