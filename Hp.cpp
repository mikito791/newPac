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
	hp = 3;
	hHp = Image::Load("Model//HP.png");
	assert(hHp >= 0);
	hHpFrame = Image::Load("Model//HPFrame.png");
	assert(hHpFrame >= 0);
	hpPos = XMFLOAT3(0.5, 0.5, 0);
}

void Hp::Update()
{
	transform_.position_ = hpPos;
	//hpFramePos = XMFLOAT3(hpPos.x - 0.5f, hpPos.y - 0.5f, hpPos.z);
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
