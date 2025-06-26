#include "Ghost.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Debug.h"
#include"Engine/SceneManager.h"

Ghost::Ghost(GameObject* parent)
	: GameObject(parent, "Ghost")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	speed = 0.05f; // �ړ����x
	distance = 0.001f; // �Փ˔���̋���
	hGhost = -1; // ���f���n���h���̏�����
}

Ghost::~Ghost()
{
}

void Ghost::Initialize()
{
	hGhost = Model::Load("Model/Ghost.fbx");
	assert(hGhost >= 0);
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

void Ghost::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
	if (transform_.position_.x < -10 || transform_.position_.x > 10 ||
		transform_.position_.z < -10 || transform_.position_.z > 10)
	{
		this->KillMe(); // ��ʊO�ɏo���玩�����폜
	}
}

void Ghost::Draw()
{
	Model::SetTransform(hGhost, transform_);
	Model::Draw(hGhost);
}

void Ghost::Release()
{
}

void Ghost::OnCollision(GameObject* pTarget)
{
}
