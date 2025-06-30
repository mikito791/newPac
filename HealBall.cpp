#include "HealBall.h"
#include "Engine/Model.h"
#include"Player.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Debug.h"
#include"Engine/SceneManager.h"
#include"Engine/SphereCollider.h"


HealBall::HealBall(GameObject* parent)
	: GameObject(parent, "HealBall")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	hHealBall = -1;
	speed = 0.05f; // �ړ����x
}
HealBall::~HealBall()
{
}

void HealBall::Initialize()
{
	hHealBall = Model::Load("Model/HealBall.fbx");
	assert(hHealBall >= 0);
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

void HealBall::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
	if (transform_.position_.x < -10 || transform_.position_.x > 10 ||
		transform_.position_.z < -10 || transform_.position_.z > 10)
	{
		this->KillMe(); // ��ʊO�ɏo���玩�����폜
	}
}

void HealBall::Draw()
{
	Model::SetTransform(hHealBall, transform_);
	Model::Draw(hHealBall);
}

void HealBall::Release()
{
}

void HealBall::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Shield")
	{
		this->KillMe(); // �ǂɏՓ˂����玩�����폜
	}
}
