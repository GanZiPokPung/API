#include "ObjectManager.h"
#include "Obj.h"

DEFINITION_SINGLE(CObjectManager)

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
	ClearPrototype();
}

bool CObjectManager::Init()
{
	return true;
}

void CObjectManager::ClearPrototype()
{
	Safe_Release_Map(m_mapPrototype);
}

CObj * CObjectManager::CreateCloneObject(const string & strProtoKey)
{
	CObj*	pProto = FindPrototype(strProtoKey);

	if (!pProto)
		return NULL;

	return pProto->Clone();
}

CObj * CObjectManager::FindPrototype(const string & strKey)
{
	unordered_map<string, class CObj*>::iterator	iter = m_mapPrototype.find(strKey);

	if (iter == m_mapPrototype.end())
		return NULL;

	return iter->second;
}
