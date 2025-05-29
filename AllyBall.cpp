#include "AllyBall.h"
#include "Engine/Model.h"
#include"Player.h"
#include"RedWall.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Debug.h"
#include"Engine/SceneManager.h"
#include"Engine/SphereCollider.h"

AllyBall::AllyBall(GameObject* parent)
	: GameObject(parent, "AllyBall")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	hAllyBall = -1;
	speed = 0.05f; // �ړ����x
}

void AllyBall::Initialize()
{
	hAllyBall = Model::Load("Model/blueEnemy0.fbx");
	assert(hAllyBall >= 0);
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

void AllyBall::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
}

void AllyBall::Draw()
{
	//if (hAllyBall < 0) return; // ���f�������[�h����Ă��Ȃ��ꍇ�͕`�悵�Ȃ�
	Model::SetTransform(hAllyBall, transform_);
	Model::Draw(hAllyBall);
}

void AllyBall::Release()
{
}

void AllyBall::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "RedWall")
	{
		moveDirection.x = -moveDirection.x; // �ǂɏՓ˂����甽�]
		moveDirection.z = -moveDirection.z; // �ǂɏՓ˂����甽�]	
	}
}
