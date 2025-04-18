#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"RedWall.h"
#include"RedEnemy.h"

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hModel(-1)
{
	
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel = Model::Load("Model/Player.fbx");
	assert(hModel >= 0);
	//transform_.position_.y = 10;
	transform_.position_=XMFLOAT3(4, 0, 2);
}

void Player::Update()
{
	if (Input::IsKeyDown(DIK_LEFT))
	{
		transform_.rotate_ = XMFLOAT3(0, 270, 0);
	}
	if (Input::IsKeyDown(DIK_RIGHT))
	{
		transform_.rotate_ = XMFLOAT3(0, 90, 0);
	}
	if (Input::IsKeyDown(DIK_UP))
	{
		transform_.rotate_ = XMFLOAT3(0, 0, 0);
	}
	if (Input::IsKeyDown(DIK_DOWN))
	{
		transform_.rotate_ = XMFLOAT3(0, 180, 0);
	}
	//�J����
	XMFLOAT3 camPos = transform_.position_;
	camPos.y = transform_.position_.y + 8.0f;
	camPos.z = transform_.position_.z - 5.0f;
	Camera::SetPosition(camPos);
	Camera::SetTarget(transform_.position_);
}

void Player::Draw()
{
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

void Player::Release()
{
}

float Player::CalculateDistanceEnemy(const XMFLOAT3& PlayPos, const XMFLOAT3& EnemyPos)
{
	//2�_�Ԃ̋������v�Z
	float dx = PlayPos.x - EnemyPos.x;
	float dy = PlayPos.y - EnemyPos.y;
	float dz = PlayPos.z - EnemyPos.z;

	//������Ԃ�(�v�Z)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}


