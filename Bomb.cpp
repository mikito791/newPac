#include "Bomb.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>

Bomb::Bomb(GameObject* parent)
	:GameObject(parent, "Bomb")
{
	hBomb = -1;
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	speed = 0.05f; // �ړ����x
	distance = 0.001f; // �Փ˔���̋���
}

Bomb::~Bomb()
{
}

void Bomb::Initialize()
{
	hBomb = Model::Load("Model/Bomb.fbx");
	assert(hBomb >= 0);
	num = rand() % 4; // 0�`3 �̃����_���l
	switch (num)
	{
	case 0: // ������
		transform_.position_ = XMFLOAT3(-2, 0, 2);
		moveDirection = XMFLOAT3(speed, 0, 0);
		break;
	case 1: // �E����
		transform_.position_ = XMFLOAT3(10, 0, 2);
		moveDirection = XMFLOAT3(-speed, 0, 0);
		break;
	case 2: // ������
		transform_.position_ = XMFLOAT3(4, 0, 6);
		moveDirection = XMFLOAT3(0, 0, -speed);
		break;
	case 3: // ��O����
		transform_.position_ = XMFLOAT3(4, 0, -4);
		moveDirection = XMFLOAT3(0, 0, speed);
		break;
	default:
		break;
	}
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collider);
}

void Bomb::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
	if (transform_.position_.x < -10 || transform_.position_.x > 10 ||
		transform_.position_.z < -10 || transform_.position_.z > 10)
	{
		this->KillMe(); // ��ʊO�ɏo���玩�����폜
	}
	transform_.rotate_.y += 3.0f;
}

void Bomb::Draw()
{
	Model::SetTransform(hBomb, transform_);
	Model::Draw(hBomb);
}

void Bomb::Release()
{
}

void Bomb::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "RedWall")
	{
		this->KillMe(); // �ǂɏՓ˂����玩�����폜
	}
	if (pTarget->GetObjectName() == "Player")
	{
		this->KillMe();
	}
}
