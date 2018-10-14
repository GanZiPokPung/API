#include "Input.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	m_pCreate(NULL)
{
}

CInput::~CInput()
{
	SAFE_RELEASE(m_pMouse);

	//반복자를 이용하여 un_map에 있는 원소들을 모두 할당 해제 후 지움
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

	// CreateKey(const T& first, const Types& ...argv) 가 불림
	CreateKey("MoveUp", 'W');
	CreateKey("MoveDown", 'S');
	CreateKey("MoveLeft", 'A');
	CreateKey('D', "MoveRight");
	CreateKey(VK_CONTROL, '1', "Skill1");
	CreateKey(VK_CONTROL, "Ctrl");
	CreateKey(VK_LBUTTON, "MouseLButton");

	CreateKey('C', "Jump");

	CreateKey(VK_RBUTTON, "MouseRButton");


	// 마우스 생성
	m_pMouse = new CMouse;

	m_pMouse->SetWindowHandle(m_hWnd);

	// 초기화 실패시 할당 해제후 false 반환
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


	//key 정보가 들어있는 un_map을 순회돌면서 키가 들어왔는지를 체크한다.
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();
	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int	iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000)
				// key 벡터에 들어있는 key값이 눌렸을때 눌린 개수에 따라 
				// pushcount 증가(동시키)
				++iPushCount;
		}

		// 벡터에 있는 키를 모두 눌렀을 경우
		// (string에 맞는 키동작이 적용되었을 경우)
		if (iPushCount == iter->second->vecKey.size())
		{
			// 누르지 않았을 경우라면 down체크를 true
			if (!iter->second->bDown && !iter->second->bPush)
				iter->second->bDown = true;
			// 이미 누르고 있을 경우 push체크를 true
			else
			{
				iter->second->bPush = true;
				iter->second->bDown = false;
			}
		}

		// 이전 프레임에 이 키를 누르고 있었을 경우
		else if (iter->second->bDown || iter->second->bPush)
		{
			iter->second->bUp = true;
			iter->second->bDown = false;
			iter->second->bPush = false;
		}

		// 키를 뗸 이후 up값 false로 대입
		else if (iter->second->bUp)
		{
			iter->second->bUp = false;
		}
	}
}

PKEYINFO CInput::FindKey(const string & strKey)	const
{
	//키를 찾아 키정보를 반환 해준다.
	unordered_map<string, PKEYINFO>::const_iterator	iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}

bool CInput::KeyDown(const string & strKey) const
{
	//키가 Down했는지를 가져온다.
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bDown;
}

bool CInput::KeyPush(const string & strKey) const
{
	//키가 Push했는지를 가져온다.
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bPush;
}

bool CInput::KeyUp(const string & strKey) const
{
	//키가 Up했는지를 가져온다.
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bUp;
}
