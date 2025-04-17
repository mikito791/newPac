#include "RedEnemy.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Player.h"
#include"RedWall.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Debug.h"
#include"Engine/SceneManager.h"

RedEnemy::RedEnemy(GameObject* parent)
	: GameObject(parent, "RedEnemy")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
}

RedEnemy::~RedEnemy()
{
}

void RedEnemy::Initialize()
{
	hRedEnemy = Model::Load("Model//RedEnemy0.fbx");
	
	num = rand() % 4 + 1; // 1�`4 �̃����_���l
	switch (num)
	{
	case 1: // ������
		transform_.position_ = XMFLOAT3(-2, 0, 2);
		moveDirection = XMFLOAT3(0.05f, 0, 0);
		break;
	case 2: // �E����
		transform_.position_ = XMFLOAT3(10, 0, 2);
		moveDirection = XMFLOAT3(-0.05f, 0, 0);
		break;
	case 3: // ������
		transform_.position_ = XMFLOAT3(4, 0, 6);
		moveDirection = XMFLOAT3(0, 0, -0.05f);
		break;
	case 4: // ��O����
		transform_.position_ = XMFLOAT3(4, 0, -4);
		moveDirection = XMFLOAT3(0, 0, 0.05f);
		break;
	}
}

void RedEnemy::Update()
{
	Player* player = (Player*)FindObject("Player");
	RedWall* redwall = (RedWall*)FindObject("RedWall");
	transform_.position_.x += moveDirection.x;
	transform_.position_.y = 0;
	transform_.position_.z += moveDirection.z;
	// �v���C���[�Ƃ̋������v�Z
	float distancePlayer = CalculateDistancePlayer(transform_.position_, player->GetPos());
	if (distancePlayer < 0.001f)
	{
		player->KillMe();
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
	}
	float distanceWall = CalculateDistanceWall(transform_.position_,redwall->GetPos());
	if (distanceWall < 0.001f)
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
	//2�_�Ԃ̋������v�Z
	float dx = EnemyPos.x - Playerpos.x;
	float dy = EnemyPos.y - Playerpos.y;
	float dz = EnemyPos.z - Playerpos.z;

	//������Ԃ�(�v�Z)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

float RedEnemy::CalculateDistanceWall(const XMFLOAT3& EnemyPos, const XMFLOAT3& Wallpos)
{
	//2�_�Ԃ̋������v�Z
	float dx = EnemyPos.x - Wallpos.x;
	float dy = EnemyPos.y - Wallpos.y;
	float dz = EnemyPos.z - Wallpos.z;
	//������Ԃ�(�v�Z)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}


