#pragma once

#include "../GameFramework.h"
#include "../Object/Mouse.h"

// 키의 종류와 상태를 가지는 struct
typedef struct _tagKeyInfo
{
	string	strName;
	bool	bDown;
	bool	bPush;
	bool	bUp;
	vector<DWORD>	vecKey;
}KEYINFO, *PKEYINFO;

class CInput
{
private:
	// 키의 상태를 value값으로 갖는 unordered_map
	unordered_map<string, PKEYINFO>		m_mapKey;
	// 초기에 map에 넣을때 필요한 임시 객체
	PKEYINFO	m_pCreate;
	// 핸들값
	HWND		m_hWnd;

private:
	//마우스 객체
	CMouse*		m_pMouse;

public:
	CMouse* GetMouse()	const
	{
		return m_pMouse;
	}

	POINT GetMousePos()	const
	{
		return m_pMouse->GetMousePos();
	}

	POINT GetMouseMove()	const
	{
		return m_pMouse->GetMouseMove();
	}

public:
	bool Init(HWND hWnd);

	// 가장 마지막 인자가 들어올 종료조건 함수를 만들어준다.
	template <typename T>
	void CreateKey(const T& data)
	{
		
		const char* pName = typeid(T).name();
		// typeid를 사용하여
		// 현재 들어온 타입이 어떤 타입인지를 알아본다.
		if (strcmp(pName, "int") == 0 ||
			strcmp(pName, "char") == 0)
		{
			//int나 char타입이라면
			//키보드 키의 값을 넣는다.
			m_pCreate->vecKey.push_back((DWORD)data);
		}
		else
		{
			m_pCreate->strName = data;
			m_mapKey.insert(make_pair(m_pCreate->strName, m_pCreate));
		}
	}

	// 키 생성될때
	template <typename T, typename ... Types>
	void CreateKey(const T& first, const Types& ...argv)
	{
		// m_pCreate가 없다면 새로 만든다.
		if (!m_pCreate)
		{
			m_pCreate = new KEYINFO;
			m_pCreate->bDown = false;
			m_pCreate->bPush = false;
			m_pCreate->bUp = false;
			m_pCreate->strName = "";
		}

		const char* pName = typeid(T).name();
		// 현재 들어온 타입이 어떤 타입인지를 알아본다.
		if (strcmp(pName, "int") == 0 ||
			strcmp(pName, "char") == 0)
		{
			//문자열이 아닐때
			m_pCreate->vecKey.push_back((DWORD)first);
		}

		else
		{
			// 이 키가 어떤 역할을 하는지에 대한 정보가 key가 되고
			// key에 대한 동적할당된 구조체 값이 value로 insert한다.
			m_pCreate->strName = first;
			m_mapKey.insert(make_pair(m_pCreate->strName, m_pCreate));
		}

		// 들어간 파라미터가 3개 이상일때 1개가 될때까지 이 함수를 재귀호출한다.
		// 동시 호출시 ex_) ctrl + 1
		CreateKey(argv...);

		// 여기로 오게되면 모든 인자에 대해서 재귀함수 호출이 끝났다는 것이다.
		// 다음에 또 써야 하므로 NULL로 초기화 시킴
		m_pCreate = NULL;
	}

	void Update(float fTime);
	PKEYINFO FindKey(const string& strKey)	const;

public:
	bool KeyDown(const string& strKey)	const;
	bool KeyPush(const string& strKey)	const;
	bool KeyUp(const string& strKey)	const;

	DECLARE_SINGLE(CInput)
};

