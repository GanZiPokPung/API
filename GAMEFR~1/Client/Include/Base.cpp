#include "Base.h"

CBase::CBase()	:
	m_iRefCount(1)		// 객체 최초 생성시 RefCnt는 1
{
}

CBase::~CBase()
{
}
