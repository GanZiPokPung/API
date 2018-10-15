#include "Layer.h"
#include "../Object/Obj.h"
#include "../Collider/CollisionManager.h"

CLayer::CLayer()	:
	// Z의 값을 통해 무엇을 먼저 출력할지를 정함
	m_iZOrder(0)
{
}

CLayer::~CLayer()
{
	// Object list를 Release
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
	// ObjList 순회돌며 Input 호출
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Input(fTime);
	}
}

void CLayer::Update(float fTime)
{
	// ObjList 순회돌며 Update 호출
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		// Enable이 false이면 건너뜀
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);

		// 죽었다면 vecList에서 Object삭제
		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
		}
		// 충돌Mgr 비교를 위한 컨테이너에 Add함
		else
		{
			GET_SINGLE(CCollisionManager)->AddObject(*iter);
			++iter;
		}
	}
}

void CLayer::Render(HDC hDC, float fTime)
{
	// ObjList 순회돌며 Render 호출
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		// Enable이 false이면 건너뜀
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		// 죽었다면 vecList에서 Object삭제
		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			continue;
		}

		// 위 상황이 아닐때 Render
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
	// ObjList 순회돌며
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		// Tag를 찾았다면 AddRef 한 후 *iter 반환
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}
