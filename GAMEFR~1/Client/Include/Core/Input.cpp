#include "Input.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	m_pCreate(NULL)
{
}

CInput::~CInput()
{
	SAFE_RELEASE(m_pMouse);

	//�ݺ��ڸ� �̿��Ͽ� un_map�� �ִ� ���ҵ��� ��� �Ҵ� ���� �� ����
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();
	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	
	m_mapKey.clear();
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	// CreateKey(const T& first, const Types& ...argv) �� �Ҹ�
	CreateKey("MoveUp", 'W');
	CreateKey("MoveDown", 'S');
	CreateKey("MoveLeft", 'A');
	CreateKey('D', "MoveRight");
	CreateKey(VK_CONTROL, '1', "Skill1");
	CreateKey(VK_CONTROL, "Ctrl");
	CreateKey(VK_LBUTTON, "MouseLButton");

	CreateKey('C', "Jump");

	CreateKey(VK_RBUTTON, "MouseRButton");


	// ���콺 ����
	m_pMouse = new CMouse;

	m_pMouse->SetWindowHandle(m_hWnd);

	// �ʱ�ȭ ���н� �Ҵ� ������ false ��ȯ
	if (!m_pMouse->Init())
	{
		SAFE_RELEASE(m_pMouse);
		return false;
	}

	return true;
}

void CInput::Update(float fTime)
{
	m_pMouse->Update(fTime);


	//key ������ ����ִ� un_map�� ��ȸ���鼭 Ű�� ���Դ����� üũ�Ѵ�.
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();
	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int	iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000)
				// key ���Ϳ� ����ִ� key���� �������� ���� ������ ���� 
				// pushcount ����(����Ű)
				++iPushCount;
		}

		// ���Ϳ� �ִ� Ű�� ��� ������ ���
		// (string�� �´� Ű������ ����Ǿ��� ���)
		if (iPushCount == iter->second->vecKey.size())
		{
			// ������ �ʾ��� ����� downüũ�� true
			if (!iter->second->bDown && !iter->second->bPush)
				iter->second->bDown = true;
			// �̹� ������ ���� ��� pushüũ�� true
			else
			{
				iter->second->bPush = true;
				iter->second->bDown = false;
			}
		}

		// ���� �����ӿ� �� Ű�� ������ �־��� ���
		else if (iter->second->bDown || iter->second->bPush)
		{
			iter->second->bUp = true;
			iter->second->bDown = false;
			iter->second->bPush = false;
		}

		// Ű�� �� ���� up�� false�� ����
		else if (iter->second->bUp)
		{
			iter->second->bUp = false;
		}
	}
}

PKEYINFO CInput::FindKey(const string & strKey)	const
{
	//Ű�� ã�� Ű������ ��ȯ ���ش�.
	unordered_map<string, PKEYINFO>::const_iterator	iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}

bool CInput::KeyDown(const string & strKey) const
{
	//Ű�� Down�ߴ����� �����´�.
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bDown;
}

bool CInput::KeyPush(const string & strKey) const
{
	//Ű�� Push�ߴ����� �����´�.
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bPush;
}

bool CInput::KeyUp(const string & strKey) const
{
	//Ű�� Up�ߴ����� �����´�.
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bUp;
}
