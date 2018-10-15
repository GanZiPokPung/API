#include "Layer.h"
#include "../Object/Obj.h"
#include "../Collider/CollisionManager.h"

CLayer::CLayer()	:
	// Z�� ���� ���� ������ ���� ��������� ����
	m_iZOrder(0)
{
}

CLayer::~CLayer()
{
	// Object list�� Release
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
	// ObjList ��ȸ���� Input ȣ��
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Input(fTime);
	}
}

void CLayer::Update(float fTime)
{
	// ObjList ��ȸ���� Update ȣ��
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		// Enable�� false�̸� �ǳʶ�
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);

		// �׾��ٸ� vecList���� Object����
		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
		}
		// �浹Mgr �񱳸� ���� �����̳ʿ� Add��
		else
		{
			GET_SINGLE(CCollisionManager)->AddObject(*iter);
			++iter;
		}
	}
}

void CLayer::Render(HDC hDC, float fTime)
{
	// ObjList ��ȸ���� Render ȣ��
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		// Enable�� false�̸� �ǳʶ�
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		// �׾��ٸ� vecList���� Object����
		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			continue;
		}

		// �� ��Ȳ�� �ƴҶ� Render
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
	// ���⿡�� �ѹ� �� ������ �ϱ� ������ ī��Ʈ�� ������Ų��.
	pObj->AddRef();
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	m_ObjList.push_back(pObj);
}

CObj * CLayer::FindObjectFromTag(const string & strTag)
{
	// ObjList ��ȸ����
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		// Tag�� ã�Ҵٸ� AddRef �� �� *iter ��ȯ
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}
