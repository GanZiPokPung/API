#include "Character.h"
#include "../Core/Input.h"

CCharacter::CCharacter()
{
}

CCharacter::CCharacter(const CCharacter & character)	:
	CDynamic(character)
{
}

CCharacter::~CCharacter()
{
}

bool CCharacter::Init()
{
	if (!CDynamic::Init())
		return false;

	return true;
}

void CCharacter::Input(float fTime)
{
	CDynamic::Input(fTime);
}

void CCharacter::Update(float fTime)
{
	CDynamic::Update(fTime);
}

void CCharacter::Render(HDC hDC, float fTime)
{
	CDynamic::Render(hDC, fTime);
}

CCharacter * CCharacter::Clone()
{
	return new CCharacter(*this);
}
