#include "Stage.h"
#include "Engine/Model.h"
Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
	
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hStage = Model::Load("Model/Stage1.fbx");
	assert(hStage >= 0);
	transform_.position_ = XMFLOAT3(4, 0, 2);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	Model::SetTransform(hStage, transform_);
	Model::Draw(hStage);
}

void Stage::Release()
{
}



