#pragma once

#include "GameFramework.h"

// 최상위 부모 객체
class CBase
{
protected:
	CBase();
	virtual ~CBase();

protected:
	int		m_iRefCount;

public:
	// this(본래) 인스턴스를 다른 인스턴스가 참조(Ref)시 RefCnt가 증가
	int AddRef()
	{
		++m_iRefCount;
		return m_iRefCount;
	}

	// 참조하는 인스턴스가 Release할때 RefCnt가 감소하고
	// 0이 되면 this(본래) 인스턴스도 해제됨
	int Release()
	{
		--m_iRefCount;
		if (m_iRefCount == 0)
		{
			delete	this;
			return 0;
		}

		return m_iRefCount;
	}
};

