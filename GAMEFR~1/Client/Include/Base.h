#pragma once

#include "GameFramework.h"

class CBase
{
protected:
	CBase();
	virtual ~CBase();

protected:
	int		m_iRefCount;

public:
	int AddRef()
	{
		++m_iRefCount;
		return m_iRefCount;
	}

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

