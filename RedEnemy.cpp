#include "RedEnemy.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Player.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Debug.h"

RedEnemy::RedEnemy(GameObject* parent)
	: GameObject(parent, "RedEnemy")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
}

RedEnemy::~RedEnemy()
{
}

void RedEnemy::Initialize()
{
	hRedEnemy = Model::Load("Model//RedEnemy0.fbx");
	
	num = rand() % 4 + 1; // 1～4 のランダム値
	switch (num)
	{
	case 1: // 左から
		transform_.position_ = XMFLOAT3(-2, 0, 2);
		moveDirection = XMFLOAT3(0.08f, 0, 0);
		break;
	case 2: // 右から
		transform_.position_ = XMFLOAT3(10, 0, 2);
		moveDirection = XMFLOAT3(-0.08f, 0, 0);
		break;
	case 3: // 奥から
		transform_.position_ = XMFLOAT3(4, 0, 6);
		moveDirection = XMFLOAT3(0, 0, -0.08f);
		break;
	case 4: // 手前から
		transform_.position_ = XMFLOAT3(4, 0, -4);
		moveDirection = XMFLOAT3(0, 0, 0.08f);
		break;
	}
	SphereCollider* collision = new SphereCollider(transform_.position_, 0.2f);
	AddCollider(collision);
}

void RedEnemy::Update()
{
	transform_.position_.x += moveDirection.x;
	if (transform_.position_.x > 15 || transform_.position_.x < -15)
	{
		this->KillMe();
	}

	transform_.position_.y = 0;

	transform_.position_.z += moveDirection.z;
	if (transform_.position_.z > 15 || transform_.position_.z < -15)
	{
		this->KillMe();
	}

	float distance = CalculateDistancePlayer(transform_.position_, ((Player*)GetParent())->GetPos());
	Debug::Log("DIS=");
	Debug::Log(distance,true);
	if (distance < 2.0f)
	{
		this->KillMe();
	}
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

float RedEnemy::CalculateDistancePlayer(const XMFLOAT3& EnemyPos, const XMFLOAT3& Playerpos)
{
	//2点間の距離を計算
	float dx = EnemyPos.x - Playerpos.x;
	float dy = EnemyPos.y - Playerpos.y;
	float dz = EnemyPos.z - Playerpos.z;

	//距離を返す(計算)
	return sqrt(dx * dx + dy * dy + dz * dz);
}

float RedEnemy::CalculateDistanceWall(const XMFLOAT3& EnemyPos, const XMFLOAT3& Wallpos)
{
	//2点間の距離を計算
	float dx = EnemyPos.x - Wallpos.x;
	float dy = EnemyPos.y - Wallpos.y;
	float dz = EnemyPos.z - Wallpos.z;
	//距離を返す(計算)
	return sqrt(dx * dx + dy * dy + dz * dz);
}


