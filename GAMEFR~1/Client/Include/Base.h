#pragma once

#include "GameFramework.h"

// �ֻ��� �θ� ��ü
class CBase
{
protected:
	CBase();
	virtual ~CBase();

protected:
	int		m_iRefCount;

public:
	// this(����) �ν��Ͻ��� �ٸ� �ν��Ͻ��� ����(Ref)�� RefCnt�� ����
	int AddRef()
	{
		++m_iRefCount;
		return m_iRefCount;
	}

	// �����ϴ� �ν��Ͻ��� Release�Ҷ� RefCnt�� �����ϰ�
	// 0�� �Ǹ� this(����) �ν��Ͻ��� ������
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

