
#include "MainScene.h"
#include "Scene.h"
#include "Layer.h"
#include "../Object/Player.h"
#include "../Object/ObjectManager.h"
#include "../Object/BackGround.h"
#include "../Core.h"
#include "../Resources/Texture.h"
#include "../Animation/Animation.h"
#include "../Object/Bullet.h"
#include "../Object/Monster.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderRect.h"
#include "../Collider/ColliderSphere.h"
#include "../Collider/ColliderPixel.h"
#include "../Object/Effect.h"
#include "../Object/UIButton.h"
#include "../Object/UICheckButton.h"
#include "../Object/UIRadioButton.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	// �⺻ ���̾ ���´�.
	CLayer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	// ������Ʈ ������Ÿ�� ����
	CObj* pPlayerProto = GET_SINGLE(CObjectManager)->CreatePrototype<CPlayer>("Player");

	pPlayerProto->UseGravity();

	CObj*	pBulletProto = GET_SINGLE(CObjectManager)->CreatePrototype<CBullet>("Bullet");

	// ��Ʈ �浹ü�� �߰��Ѵ�.
	POSITION	tPivot(0.5f, 0.5f);
	POSITION	tPos = pBulletProto->GetPos() - (pBulletProto->GetPivot() - tPivot) * pBulletProto->GetSize();
	CColliderSphere*	pSphere = CColliderSphere::Create("Bullet", tPos.x, tPos.y, pBulletProto->GetSize().x / 2.f);

	pBulletProto->AddCollider(pSphere);

	// ���� ����Ʈ ������Ÿ��
	CObj*	pFireEffectProto = GET_SINGLE(CObjectManager)->CreatePrototype<CEffect>("FireEffect");

	pFireEffectProto->SetTexture("FireEffect", L"player_bomb.bmp", true);

	pFireEffectProto->SetAnimation("FireEffect.anm");

	pFireEffectProto->SetAnimationFrameSize();

	// ������Ÿ���� �̿��Ͽ� ���� ��밴ü�� ������ش�.
	CObj*	pPlayer = GET_SINGLE(CObjectManager)->CreateCloneObject("Player");

	// ��Ʈ �浹ü�� �߰��Ѵ�.
	tPos = pPlayer->GetPos() - pPlayer->GetPivot() * pPlayer->GetSize();
	CColliderRect* pRC	= CColliderRect::Create("PlayerBody", tPos.x, tPos.y, tPos.x + pPlayer->GetSize().x,
		tPos.y + pPlayer->GetSize().y);

	pPlayer->AddCollider(pRC);

	pRC->AddCollisionFunction<CPlayer>(CFT_ENTER, &CPlayer::CollisionEnter, (CPlayer*)pPlayer);
	pRC->AddCollisionFunction<CPlayer>(CFT_TRIGGER, &CPlayer::CollisionTrigger, (CPlayer*)pPlayer);

	// AddObject �Լ� �ȿ��� ���۷��� ī��Ʈ�� 1 �����ؼ� 2�� �Ǿ��ִ�.
	// �׷��� ������ ���⿡�� pPlayer ��ü�� ��� ����ߴٸ� Release ���־�� �Ѵ�.
	pLayer->AddObject(pPlayer);

	// ī�޶��� Ÿ������ �÷��̾ �ش�.
	GET_SINGLE(CCamera)->SetTarget(pPlayer);

	SAFE_RELEASE(pPlayer);

	CLayer*	pMapLayer = m_pScene->FindLayer("MapLayer");

	CObj*	pMap = GET_SINGLE(CObjectManager)->CreateObject<CBackGround>("Map");

	pMap->SetPos(0.f, 0.f);
	//pMap->SetSize(RESOLUTION_W, RESOLUTION_H);
	pMap->SetPivot(0.f, 0.f);

	// �̹����� �����Ѵ�.
	pMap->SetTexture("BackGround", L"Back1_Collision.bmp");

	// �� �̹����� ũ��� �������� ũ�⸦ �����Ѵ�.
	CTexture*	pTexture = pMap->GetTexture();
	GET_SINGLE(CCamera)->SetResolution(pTexture->GetWidth(), pTexture->GetHeight());

	SAFE_RELEASE(pTexture);

	CColliderPixel* pPixel = CColliderPixel::Create("MapCollider", "MapCollider", "Back1_Collision.bmp",
		255, 0, 255);

	pMap->AddCollider(pPixel);

	pMapLayer->AddObject(pMap);

	SAFE_RELEASE(pMap);

	// ���� ������Ÿ�� ����
	CObj* pMonsterProto = GET_SINGLE(CObjectManager)->CreatePrototype<CMonster>("Monster");

	CObj*	pMonster = GET_SINGLE(CObjectManager)->CreateCloneObject("Monster");
	
	pLayer->AddObject(pMonster);

	SAFE_RELEASE(pMonster);

	pMonster = GET_SINGLE(CObjectManager)->CreateCloneObject("Monster");

	pMonster->SetPos(1000.f, 200.f);

	pLayer->AddObject(pMonster);

	SAFE_RELEASE(pMonster);

	// �׽�Ʈ�� ��ư ����
	CObj*	pButton = GET_SINGLE(CObjectManager)->CreateObject<CUIButton>("StartButton");

	pButton->SetPos(RESOLUTION_W / 2.f, RESOLUTION_H / 2.f);
	pButton->SetTexture("StartButton", L"StartButton.bmp");

	pButton->SetSizeFromImageSize();
	
	CLayer*	pUILayer = m_pScene->FindLayer("UILayer");

	pLayer->AddObject(pButton);

	SAFE_RELEASE(pButton);

	// �׽�Ʈ�� ��ư ����

	CObj*	pCheckButton = GET_SINGLE(CObjectManager)->CreateObject<CUICheckButton>("CheckButton");

	pCheckButton->SetPos(RESOLUTION_W / 4.f, RESOLUTION_H / 4.f);
	pCheckButton->SetTexture("CheckButton", L"CheckButton.bmp");

	pCheckButton->SetSizeFromImageSize();

	//pUILayer = m_pScene->FindLayer("UILayer");

	pLayer->AddObject(pCheckButton);

	SAFE_RELEASE(pCheckButton);

	CObj*	pRadioButton = GET_SINGLE(CObjectManager)->CreateObject<CUIRadioButton>("RadioButton");

	pRadioButton->SetPos(RESOLUTION_W / 1.5f, RESOLUTION_H / 6.f);
	pRadioButton->SetTexture("RadioButton", L"RadioButton.bmp");

	pRadioButton->SetSizeFromImageSize();

	//pUILayer = m_pScene->FindLayer("UILayer");

	pLayer->AddObject(pRadioButton);

	SAFE_RELEASE(pRadioButton);

	return true;
}

void CMainScene::Input(float fTime)
{
}

void CMainScene::Update(float fTime)
{
}

void CMainScene::Render(HDC hDC, float fTime)
{
}
