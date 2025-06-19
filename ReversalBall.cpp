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
	speed(0.05f) // �ړ����x
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
}

ReversalBall::~ReversalBall()
{
}

void ReversalBall::Initialize()
{
	hModel = Model::Load("Model/redEnemy0.fbx");
	assert(hModel >= 0);
	randomNum = rand() % 4; // 0�`3 �̃����_���l
	switch (randomNum)
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



void ReversalBall::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
	if (transform_.position_.x < -10 || transform_.position_.x > 10 ||
		transform_.position_.z < -10 || transform_.position_.z > 10)
	{
		this->KillMe(); // ��ʊO�ɏo���玩�����폜
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
		this->KillMe(); // �ǂɏՓ˂����玩�����폜
	}
}

Direction ReversalBall::GetReveralDirectionFromInput()
{
	//����𔽓]������
	static Direction lastDirection = FRONT;

	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = RIGHT;//�����͂�����ƉE������
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = LEFT;//�E���͂�����ƍ�������
	if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = BACK;   //����͂�����ƌ�������
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = FRONT;//�����͂�����ƑO������

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
