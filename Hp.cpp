#include "Hp.h"
#include"Engine/Image.h"

Hp::Hp(GameObject* parent) 
	: GameObject(parent, "Hp")
{
}

Hp::~Hp()
{
}

void Hp::Initialize()
{
	hp = 5;
	hHp = Image::Load("Model//HP.png");
	assert(hHp >= 0);
	hHpFrame = Image::Load("Model//HPFrame.png");
	assert(hHpFrame >= 0);
}

void Hp::Update()
{
}

void Hp::Draw()
{
	Image::SetTransform(hHpFrame, transform_);
	Image::Draw(hHpFrame);
	Image::SetTransform(hHp, transform_);
	Image::Draw(hHp);
	
}
void Hp::Release()
{
}
