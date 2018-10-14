#pragma once

#include "../GameFramework.h"
#include "../Object/Mouse.h"

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
	unordered_map<string, PKEYINFO>		m_mapKey;
	PKEYINFO	m_pCreate;
	HWND		m_hWnd;

private:
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

	// ���� ������ ���ڰ� ���� �������� �Լ��� ������ش�.
	template <typename T>
	void CreateKey(const T& data)
	{
		const char* pName = typeid(T).name();
		// ���� ���� Ÿ���� � Ÿ�������� �˾ƺ���.
		if (strcmp(pName, "int") == 0 ||
			strcmp(pName, "char") == 0)
		{
			m_pCreate->vecKey.push_back((DWORD)data);
		}

		else
		{
			m_pCreate->strName = data;
			m_mapKey.insert(make_pair(m_pCreate->strName, m_pCreate));
		}
	}

	template <typename T, typename ... Types>
	void CreateKey(const T& first, const Types& ...argv)
	{
		if (!m_pCreate)
		{
			m_pCreate = new KEYINFO;
			m_pCreate->bDown = false;
			m_pCreate->bPush = false;
			m_pCreate->bUp = false;
			m_pCreate->strName = "";
		}

		const char* pName = typeid(T).name();
		// ���� ���� Ÿ���� � Ÿ�������� �˾ƺ���.
		if (strcmp(pName, "int") == 0 ||
			strcmp(pName, "char") == 0)
		{
			m_pCreate->vecKey.push_back((DWORD)first);
		}

		else
		{
			m_pCreate->strName = first;
			m_mapKey.insert(make_pair(m_pCreate->strName, m_pCreate));
		}

		CreateKey(argv...);

		// ����� ���ԵǸ� ��� ���ڿ� ���ؼ� ����Լ� ȣ���� �����ٴ� ���̴�.
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
