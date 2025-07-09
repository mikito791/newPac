#include "Shield.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Engine/Input.h"


namespace
{
	// 反転の持続時間（秒）
	const float reversalDuration = 5.0f;
}
Shield::Shield(GameObject* parent): 
	GameObject(parent, "Shield"),hShield(-1)
{
	
}

Shield::~Shield()
{
}

void Shield::Initialize()
{
	hShield = Model::Load("Model/Shield.fbx");
	assert(hShield >= 0);
	
	SphereCollider* collision = new SphereCollider(transform_.position_, 0.01f);
	AddCollider(collision);
	
	csv.Load("CSV/Player.csv");
}

void Shield::Update()
{
	float deltaTime = GetDeltaTime();
	//入力処理
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection); // 向きを更新
	transform_.position_ = GetPositionFromDirection(currentDirection); // 位置を更新
	if (onReversal)
	{
		reversalTimer += deltaTime;
		if (reversalTimer >= reversalDuration)
		{
			onReversal = false;
			reversalTimer = 0.0f;
		}
	}
}

void Shield::Draw()
{
	Model::SetTransform(hShield, transform_);
	Model::Draw(hShield);
}

void Shield::Release()
{
}

Direction Shield::GetDirectionFromInput()const
{
	static Direction lastDirection;

	if (!onReversal)
	{
		if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
		if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
		if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
		if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;
	}
	else 
	{
		if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = RIGHT;
		if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = LEFT;
		if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = BACK;
		if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = FRONT;
	}

	return lastDirection;
}

int Shield::GetRotationFromDirection(Direction dir)
{
	int Left, Right, Front, Back;
	switch (dir)
	{
	case LEFT:
		return Left = csv.GetValue(1, 0);
		//return 270; // 左向き
		break;
	case RIGHT:
		return Right = csv.GetValue(2, 0);
		//return 90; // 右向き
		break;
	case FRONT:
		return Front = csv.GetValue(3, 0);
		//return 0; // 前向き
		break;
	case BACK:
		return Back = csv.GetValue(4, 0);
		//return 180;  //後ろ向き
		break;
	default:
		break;
	}
}

XMFLOAT3 Shield::GetPositionFromDirection(Direction dir)
{
	//csvで位置を取得できるようにする
	float x, y, z;
	switch (dir)
	{
	case LEFT:
		x = csv.GetValue(1, 1);
		y = csv.GetValue(2, 1);
		z = csv.GetValue(3, 1);
		return XMFLOAT3(x, y, z);
		break;
	case RIGHT:
		x = csv.GetValue(4, 1);
		y = csv.GetValue(2, 1);
		z = csv.GetValue(3, 1);
		return XMFLOAT3(x, y, z);
		break;
	case FRONT:
		x = csv.GetValue(1, 2);
		y = csv.GetValue(2, 2);
		z = csv.GetValue(3, 2);
		return XMFLOAT3(x, y, z);
		break;
	case BACK:
		x = csv.GetValue(1, 2);
		y = csv.GetValue(2, 2);
		z = csv.GetValue(4, 2);
		return XMFLOAT3(x, y, z);
		break;
	default:
		break;
	} 
}

float Shield::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}


