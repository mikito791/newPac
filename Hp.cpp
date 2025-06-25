#include "Hp.h"
#include"Engine/Image.h"

Hp::Hp(GameObject* parent) 
	: GameObject(parent, "Hp")
{
	hHp = -1;
	hHpFrame = -1;
	hpMaxVal_ = 100; // Å‘å’l‚Ì‰Šú‰»
	hpCrrVal_ = 100; // Œ»Ý’l‚Ì‰Šú‰»
}

Hp::~Hp()
{
}

void Hp::Initialize()
{
	
	hHp = Image::Load("Model//HP.png");
	assert(hHp >= 0);
	hHpFrame = Image::Load("Model//HPFrame.png");
	assert(hHpFrame >= 0);
	transform_.position_.x = -1.0f;
	transform_.position_.y = 0.9f;
	
}

void Hp::Update()
{
}

void Hp::Draw()
{
	Transform tr = transform_;
	tr.scale_.x = (float)hpCrrVal_ / (float)hpMaxVal_;
	Image::SetTransform(hHpFrame, transform_);
	Image::Draw(hHpFrame);
	Image::SetTransform(hHp, tr);
	Image::Draw(hHp);
	
}
void Hp::Release()
{
}
