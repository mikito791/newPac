#include "Hp.h"

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
}

void Hp::Update()
{
}

void Hp::Draw()
{
}

void Hp::Release()
{
}
