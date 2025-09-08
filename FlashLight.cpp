#include "FlashLight.h"
#include"Engine/Model.h"

FlashLight::FlashLight(GameObject* parent)
{
	hFlashLight = -1;
}

FlashLight::~FlashLight()
{
}

void FlashLight::Initialize()
{
	hFlashLight = Model::Load("Model/Light.fbx");
	assert(hFlashLight >= 0);
}

void FlashLight::Update()
{
}

void FlashLight::Draw()
{
}

void FlashLight::Release()
{
}
