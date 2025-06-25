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
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
	speed = 0.05f; // 移動速度
	distance = 0.001f; // 衝突判定の距離
}

Bomb::~Bomb()
{
}

void Bomb::Initialize()
{
	hBomb = Model::Load("Model/Bomb.fbx");
	assert(hBomb >= 0);
	num = rand() % 4; // 0～3 のランダム値
	switch (num)
	{
	case 0: // 左から
		transform_.position_ = XMFLOAT3(-2, 0, 2);
		moveDirection = XMFLOAT3(speed, 0, 0);
		break;
	case 1: // 右から
		transform_.position_ = XMFLOAT3(10, 0, 2);
		moveDirection = XMFLOAT3(-speed, 0, 0);
		break;
	case 2: // 奥から
		transform_.position_ = XMFLOAT3(4, 0, 6);
		moveDirection = XMFLOAT3(0, 0, -speed);
		break;
	case 3: // 手前から
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
		this->KillMe(); // 画面外に出たら自分を削除
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
		this->KillMe(); // 壁に衝突したら自分を削除
	}
	if (pTarget->GetObjectName() == "Player")
	{
		this->KillMe();
	}
}
