#include "NeedleBall.h"
#include"Engine/Model.h"
#include"Player.h"
#include"RedWall.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Debug.h"
#include"Engine/SceneManager.h"
#include"Engine/SphereCollider.h"
#include"Engine/Input.h"

NeedleBall::NeedleBall(GameObject* parent)
	: GameObject(parent, "NeedleBall")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	speed = 0.05f; // �ړ����x
	distance = 0.001f; // �Փ˔���̋���
}

NeedleBall::~NeedleBall()
{
	
}

void NeedleBall::Initialize()
{
	hNeedleBall = Model::Load("Model//Needle.fbx");
	assert(hNeedleBall >= 0);
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

void NeedleBall::Update()
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

void NeedleBall::Draw()
{
	Model::SetTransform(hNeedleBall, transform_);
	Model::Draw(hNeedleBall);
}

void NeedleBall::Release()
{
}

float NeedleBall::CalculateDistancePlayer(const XMFLOAT3& EnemyPos, const XMFLOAT3& Playerpos)
{
	//2�_�Ԃ̋������v�Z
	float dx = EnemyPos.x - Playerpos.x;
	float dy = EnemyPos.y - Playerpos.y;
	float dz = EnemyPos.z - Playerpos.z;

	//������Ԃ�(�v�Z)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

float NeedleBall::CalculateDistanceWall(const XMFLOAT3& EnemyPos, const XMFLOAT3& Wallpos)
{
	//2�_�Ԃ̋������v�Z
	float dx = EnemyPos.x - Wallpos.x;
	float dy = EnemyPos.y - Wallpos.y;
	float dz = EnemyPos.z - Wallpos.z;
	//������Ԃ�(�v�Z)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

void NeedleBall::OnCollision(GameObject* pTarget)
{
	// �ǂƂ̏Փ˔���
	if (pTarget->GetObjectName() == "RedWall")
	{
		moveDirection.x = -moveDirection.x; // �ǂɏՓ˂����甽�]
		moveDirection.z = -moveDirection.z; // �ǂɏՓ˂����甽�]
	}
}
