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
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
	hHealBall = -1;
	speed = 0.05f; // 移動速度
}
HealBall::~HealBall()
{
}

void HealBall::Initialize()
{
	hHealBall = Model::Load("Model/HealBall.fbx");
	assert(hHealBall >= 0);
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

void HealBall::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
	if (transform_.position_.x < -10 || transform_.position_.x > 10 ||
		transform_.position_.z < -10 || transform_.position_.z > 10)
	{
		this->KillMe(); // 画面外に出たら自分を削除
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
		this->KillMe(); // 壁に衝突したら自分を削除
	}
}
