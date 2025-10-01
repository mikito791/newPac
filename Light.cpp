#include "Light.h"
#include"Engine/Model.h"

Light::Light(GameObject* parent)
	:GameObject(parent, "Light")
{
	hLight = -1;
}

Light::~Light()
{
}

void Light::Initialize()
{
	hLight = Model::Load("Model/Light.fbx");
	assert(hLight >= 0);
}

void Light::Update()
{
}

void Light::Draw()
{
	Model::SetTransform(hLight, transform_);
	Model::Draw(hLight);
}

void Light::Release()
{
}
