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
	hRedEnemy = Model::Load("Model//RedEnemy0.fbx");
	SphereCollider* collision = new SphereCollider(transform_.position_, 0.2f);
	AddCollider(collision);
	transform_.position_ = XMFLOAT3(0, 0, 2);
}

void RedEnemy::Update()
{
	transform_.position_.x += 0.05f;
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
