#include "RedWall.h"
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

RedWall::RedWall(GameObject* parent): 
	GameObject(parent, "RedWall"),hRedWall(-1)
{
	
}

RedWall::~RedWall()
{
}

void RedWall::Initialize()
{
	hRedWall = Model::Load("Model/Shield.fbx");
	assert(hRedWall >= 0);
	
	SphereCollider* collision = new SphereCollider(transform_.position_, 0.01f);
	AddCollider(collision);
	
	csv.Load("CSV/variable.csv");
}

void RedWall::Update()
{
	//入力処理
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection); // 向きを更新
	transform_.position_ = GetPositionFromDirection(currentDirection); // 位置を更新
}

void RedWall::Draw()
{
	Model::SetTransform(hRedWall, transform_);
	Model::Draw(hRedWall);
}

void RedWall::Release()
{
}

Direction RedWall::GetDirectionFromInput()
{
	static Direction lastDirection;

	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
	if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;

	return lastDirection;
}

int RedWall::GetRotationFromDirection(Direction dir)
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

XMFLOAT3 RedWall::GetPositionFromDirection(Direction dir)
{
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


