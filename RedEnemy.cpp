#include "RedEnemy.h"
#include"Engine/Model.h"
#include"Player.h"
#include"RedWall.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Debug.h"
#include"Engine/SceneManager.h"
#include"Engine/SphereCollider.h"



RedEnemy::RedEnemy(GameObject* parent)
	: GameObject(parent, "RedEnemy")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	speed = 0.05f; // �ړ����x
	distance = 0.001f; // �Փ˔���̋���
}

RedEnemy::~RedEnemy()
{
}

void RedEnemy::Initialize()
{
	hRedEnemy = Model::Load("Model//RedEnemy0.fbx");
	
	num = rand() % 4; // 0�`3 �̃����_���l
	DirState = num;
	switch (num)
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
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0), 0.3f);
	AddCollider(collider);
}

void RedEnemy::Update()
{
	Player* player = (Player*)FindObject("Player");
	RedWall* redwall = (RedWall*)FindObject("RedWall");
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
	// �v���C���[�Ƃ̋������v�Z
	float distancePlayer = CalculateDistancePlayer(transform_.position_, player->GetPos());
	if (distancePlayer < distance)
	{
		player->KillMe();
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMEOVER);
	}
	// �ǂƂ̋������v�Z
	float distanceWall = CalculateDistanceWall(transform_.position_,redwall->GetPos());
	if (distanceWall < distance)
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

void RedEnemy::OnCollision(GameObject* pTarget)
{
	// �v���C���[�Ƃ̏Փ˔���
	if (pTarget->GetObjectName() == "Player")
	{
		pTarget->KillMe();
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMEOVER);
	}
	// �ǂƂ̏Փ˔���
	if (pTarget->GetObjectName() == "RedWall")
	{
		this->KillMe();
	}
}


