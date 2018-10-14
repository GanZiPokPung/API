#include "Layer.h"
#include "../Object/Obj.h"
#include "../Collider/CollisionManager.h"

CLayer::CLayer()	:
	m_iZOrder(0)
{
}

CLayer::~CLayer()
{
	Safe_Release_VecList(m_ObjList);
}

bool CLayer::Init(const string& strName, int iZOrder)
{
	m_strName = strName;
	m_iZOrder = iZOrder;

	return true;
}

void CLayer::Input(float fTime)
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Input(fTime);
	}
}

void CLayer::Update(float fTime)
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
		}

		else
		{
			GET_SINGLE(CCollisionManager)->AddObject(*iter);
			++iter;
		}
	}
}

void CLayer::Render(HDC hDC, float fTime)
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			continue;
		}

		(*iter)->Render(hDC, fTime);
		++iter;
	}
}

void CLayer::Destroy()
{
	delete	this;
}

void CLayer::AddObject(CObj * pObj)
{
	// 여기에서 한번 더 참조를 하기 때문에 카운트를 증가시킨다.
	pObj->AddRef();
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	m_ObjList.push_back(pObj);
}

CObj * CLayer::FindObjectFromTag(const string & strTag)
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}
