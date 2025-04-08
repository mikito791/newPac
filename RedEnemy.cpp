#include "RedEnemy.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include <cstdlib> // for rand()
#include<ctime>

RedEnemy::RedEnemy(GameObject* parent)
	: GameObject(parent, "RedEnemy")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
}

RedEnemy::~RedEnemy()
{
}

void RedEnemy::Initialize()
{
	hRedEnemy = Model::Load("Model//RedEnemy0.fbx");
	/*SphereCollider* collision = new SphereCollider(transform_.position_, 0.2f);
	AddCollider(collision);*/
	num = rand() % 4 + 1; // 1�`4 �̃����_���l
	switch (num)
	{
	case 1: // ������
		transform_.position_ = XMFLOAT3(-2, 0, 2);
		moveDirection = XMFLOAT3(0.05f, 0, 0);
		break;
	case 2: // �E����
		transform_.position_ = XMFLOAT3(10, 0, 2);
		moveDirection = XMFLOAT3(-0.05f, 0, 0);
		break;
	case 3: // ������
		transform_.position_ = XMFLOAT3(4, 0, 6);
		moveDirection = XMFLOAT3(0, 0, -0.05f);
		break;
	case 4: // ��O����
		transform_.position_ = XMFLOAT3(4, 0, -4);
		moveDirection = XMFLOAT3(0, 0, 0.05f);
		break;
	}
}

void RedEnemy::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
}

void RedEnemy::Draw()
{
	Model::SetTransform(hRedEnemy,transform_);
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
