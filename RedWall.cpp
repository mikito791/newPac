#include "RedWall.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"

RedWall::RedWall(GameObject* parent): 
	GameObject(parent, "Wall"),hRedWall(-1),hBlueWall(-1)
{
	transform_.position_ = XMFLOAT3(4, 0, 2);
}

RedWall::~RedWall()
{
}

void RedWall::Initialize()
{
	hRedWall = Model::Load("Model/PlayerWallred.fbx");
	hBlueWall = Model::Load("Model/PlayerWallblue.fbx");
}

void RedWall::Update()
{
}

void RedWall::Draw()
{
	Model::SetTransform(hRedWall, transform_);
	Model::Draw(hRedWall);
	Model::SetTransform(hBlueWall, transform_);
	Model::Draw(hBlueWall);
}

void RedWall::Release()
{
}
