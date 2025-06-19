#include "ReversalBall.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>


ReversalBall::ReversalBall(GameObject* parent)
	: GameObject(parent, "ReversalBall"),
	hModel(-1),
	randomNum(0),
	moveDirection(XMFLOAT3(0, 0, 0)),
	speed(0.05f) // 移動速度
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
}

ReversalBall::~ReversalBall()
{
}

void ReversalBall::Initialize()
{
	hModel = Model::Load("Model/redEnemy0.fbx");
	assert(hModel >= 0);
	randomNum = rand() % 4; // 0～3 のランダム値
	switch (randomNum)
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



void ReversalBall::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
	if (transform_.position_.x < -10 || transform_.position_.x > 10 ||
		transform_.position_.z < -10 || transform_.position_.z > 10)
	{
		this->KillMe(); // 画面外に出たら自分を削除
	}
}

void ReversalBall::Draw()
{
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

void ReversalBall::Release()
{
}

void ReversalBall::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "RedWall")
	{
		this->KillMe(); // 壁に衝突したら自分を削除
	}
}

Direction ReversalBall::GetReveralDirectionFromInput()
{
	//操作を反転させる
	static Direction lastDirection = FRONT;

	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = RIGHT;//左入力をすると右を向く
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = LEFT;//右入力をすると左を向く
	if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = BACK;   //上入力をすると後ろを向く
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = FRONT;//下入力をすると前を向く

	return lastDirection;
}

int ReversalBall::GetRotationFromReveralDirection(Direction dir)
{
	switch (dir)
	{
	case LEFT:
		return 270;
		break;
	case RIGHT:
		return 90;
		break;
	case FRONT:
		return 0;
		break;
	case BACK:
		return 180;
		break;
	default:
		break;
	}
}

XMFLOAT3 ReversalBall::GetPositionFromReveralDirection(Direction dir)
{
	switch (dir)
	{
	case LEFT:
		return XMFLOAT3(3, 0, 2);
		break;
	case RIGHT:
		return XMFLOAT3(5, 0, 2);
		break;
	case FRONT:
		return XMFLOAT3(4, 0, 3);
		break;
	case BACK:
		return XMFLOAT3(4, 0, 1);
		break;
	default:
		break;
	}
}
