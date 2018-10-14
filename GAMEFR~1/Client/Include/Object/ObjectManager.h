#pragma once

#include "../GameFramework.h"

class CObjectManager
{
private:
	unordered_map<string, class CObj*>	m_mapPrototype;

public:
	bool Init();
	void ClearPrototype();

public:
	template <typename T>
	CObj* CreatePrototype(const string& strKey)
	{
		CObj*	pProto = FindPrototype(strKey);

		if (pProto)
			return pProto;

		pProto = new T;

		if (!pProto->Init())
		{
			SAFE_RELEASE(pProto);
			return NULL;
		}

		pProto->SetTag(strKey);

		m_mapPrototype.insert(make_pair(strKey, pProto));

		return pProto;
	}

	template <typename T>
	CObj* CreateObject(const string& strName)
	{
		CObj*	pObj = new T;
		
		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}

		return pObj;
	}

	CObj* CreateCloneObject(const string& strProtoKey);

private:
	class CObj* FindPrototype(const string& strKey);

	DECLARE_SINGLE(CObjectManager)
};

