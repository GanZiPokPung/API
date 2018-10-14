#pragma once

#include "../GameFramework.h"
#include "../Object/Mouse.h"

// Ű�� ������ ���¸� ������ struct
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
	// Ű�� ���¸� value������ ���� unordered_map
	unordered_map<string, PKEYINFO>		m_mapKey;
	// �ʱ⿡ map�� ������ �ʿ��� �ӽ� ��ü
	PKEYINFO	m_pCreate;
	// �ڵ鰪
	HWND		m_hWnd;

private:
	//���콺 ��ü
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
		// typeid�� ����Ͽ�
		// ���� ���� Ÿ���� � Ÿ�������� �˾ƺ���.
		if (strcmp(pName, "int") == 0 ||
			strcmp(pName, "char") == 0)
		{
			//int�� charŸ���̶��
			//Ű���� Ű�� ���� �ִ´�.
			m_pCreate->vecKey.push_back((DWORD)data);
		}
		else
		{
			m_pCreate->strName = data;
			m_mapKey.insert(make_pair(m_pCreate->strName, m_pCreate));
		}
	}

	// Ű �����ɶ�
	template <typename T, typename ... Types>
	void CreateKey(const T& first, const Types& ...argv)
	{
		// m_pCreate�� ���ٸ� ���� �����.
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
			//���ڿ��� �ƴҶ�
			m_pCreate->vecKey.push_back((DWORD)first);
		}

		else
		{
			// �� Ű�� � ������ �ϴ����� ���� ������ key�� �ǰ�
			// key�� ���� �����Ҵ�� ����ü ���� value�� insert�Ѵ�.
			m_pCreate->strName = first;
			m_mapKey.insert(make_pair(m_pCreate->strName, m_pCreate));
		}

		// �� �Ķ���Ͱ� 3�� �̻��϶� 1���� �ɶ����� �� �Լ��� ���ȣ���Ѵ�.
		// ���� ȣ��� ex_) ctrl + 1
		CreateKey(argv...);

		// ����� ���ԵǸ� ��� ���ڿ� ���ؼ� ����Լ� ȣ���� �����ٴ� ���̴�.
		// ������ �� ��� �ϹǷ� NULL�� �ʱ�ȭ ��Ŵ
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

