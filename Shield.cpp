#include "Shield.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Engine/Input.h"
namespace
{
	XMFLOAT3 Left(3, 0, 2);
	XMFLOAT3 Right(5, 0, 2);
	XMFLOAT3 Back(4, 0, 1);
	XMFLOAT3 Front(4, 0, 3);
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
	
	csv.Load("CSV/variable.csv");
}

void Shield::Update()
{
	//入力処理
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection); // 向きを更新
	transform_.position_ = GetPositionFromDirection(currentDirection); // 位置を更新
}

void Shield::Draw()
{
	Model::SetTransform(hShield, transform_);
	Model::Draw(hShield);
}

void Shield::Release()
{
}

Direction Shield::GetDirectionFromInput()
{
	static Direction lastDirection;

	if (!pPlayer) return lastDirection; // セーフガード

	bool isReversed = pPlayer->IsOnReversal();

	if (!isReversed) 
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
		break;
	case RIGHT:
		return Right = csv.GetValue(2, 0);
		break;
	case FRONT:
		return Front = csv.GetValue(3, 0);
		break;
	case BACK:
		return Back = csv.GetValue(4, 0);
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
		return Left;
		break;
	case RIGHT:
		return Right;
		break;
	case FRONT:
		return Front;
		break;
	case BACK:
		return Back;
		break;
	default:
		break;
	}
}


