#include "RedEnemy.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"

RedEnemy::RedEnemy(GameObject* parent)
	: GameObject(parent, "RedEnemy")
{

}

RedEnemy::~RedEnemy()
{
}

void RedEnemy::Initialize()
{
	hRedEnemy = Model::Load("Model//RedEnemy.fbx");
	pos=transform_.position_ = { 0.0f,0.0f,0.5f };
	SphereCollider* collision = new SphereCollider(pos, 0.5f);
	AddCollider(collision);
}

void RedEnemy::Update()
{
	transform_.position_.x += 0.01f;
}

void RedEnemy::Draw()
{
	Model::SetTransform(hRedEnemy, transform_);
	Model::Draw(hRedEnemy);
}

void RedEnemy::Release()
{
}

void RedEnemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "RedWall")
	{
		this->KillMe();
	}
}
