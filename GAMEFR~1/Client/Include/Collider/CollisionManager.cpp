#include "CollisionManager.h"
#include "../Object/Obj.h"
#include "Collider.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

bool CCollisionManager::Init()
{
	m_vecObj.reserve(1000);

	return true;
}

void CCollisionManager::AddObject(CObj * pObj)
{
	if (!pObj->CheckCollider())
		return;

	m_vecObj.push_back(pObj);
}

void CCollisionManager::Collision(float fTime)
{
	if (m_vecObj.size() < 2)
	{
		m_vecObj.clear();
		return;
	}

	for (size_t i = 0; i < m_vecObj.size() - 1; ++i)
	{
		for (size_t j = i + 1; j < m_vecObj.size(); ++j)
		{
			CollisionObj(m_vecObj[i], m_vecObj[j]);
		}
	}

	m_vecObj.clear();
}

void CCollisionManager::CollisionObj(CObj * pObj1, CObj * pObj2)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = pObj1->m_ColliderList.end();
	list<CCollider*>::iterator	iter1;
	list<CCollider*>::iterator	iter1End = pObj2->m_ColliderList.end();

	for (iter = pObj1->m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		for (iter1 = pObj2->m_ColliderList.begin(); iter1 != iter1End; ++iter1)
		{
			if ((*iter)->Collision(*iter1))
			{
				// 충돌 리스트에 없을 경우 처음 충돌 됐다는 의미이다.
				if (!(*iter)->CheckCollisionList(*iter1))
				{
					(*iter)->AddCollisionCollider(*iter1);
					(*iter1)->AddCollisionCollider(*iter);

					(*iter)->CollisionEnter(*iter1);
					(*iter1)->CollisionEnter(*iter);
				}

				// 이전 프레임에 충돌이 되었다.
				else
				{
					(*iter)->CollisionTrigger(*iter1);
					(*iter1)->CollisionTrigger(*iter);
				}
			}
			
			else
			{
				// 충돌이 떨어진 상태에서 기존에 충돌이 되었다면 막 충돌되다가 떨어진 것이다.
				if ((*iter)->CheckCollisionList(*iter1))
				{
					(*iter)->DeleteCollisionList(*iter1);
					(*iter1)->DeleteCollisionList(*iter);

					(*iter)->CollisionLeave(*iter1);
					(*iter1)->CollisionLeave(*iter);
				}
			}
		}
	}
}
