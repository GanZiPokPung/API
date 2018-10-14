#include "BackGround.h"
#include "../Core/Camera.h"
#include "../Core.h"
#include "../Resources/Texture.h"

CBackGround::CBackGround()
{
}

CBackGround::CBackGround(const CBackGround & back)	:
	CObj(back)
{
}

CBackGround::~CBackGround()
{
}

bool CBackGround::Init()
{
	return true;
}

void CBackGround::Input(float fTime)
{
}

void CBackGround::Update(float fTime)
{
}

void CBackGround::Render(HDC hDC, float fTime)
{
	//CObj::Render(hDC, fTime);
	POSITION	tCamPos = GET_SINGLE(CCamera)->GetPos();

	TransparentBlt(hDC, 0, 0, RESOLUTION_W, RESOLUTION_H, 
		m_pTexture->GetMemDC(), tCamPos.x, tCamPos.y,
		RESOLUTION_W, RESOLUTION_H, RGB(0, 0, 0));
}

CBackGround * CBackGround::Clone()
{
	return new CBackGround(*this);
}
