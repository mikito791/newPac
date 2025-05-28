#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"RedWall.h"
#include"RedEnemy.h"
#include"Hp.h"
#include"Engine/Debug.h"
#include"AllyBall.h"

namespace
{
	int HP = 50;
	float invincibilityTime = 2.0f;
	bool isInvincible = false;
	float invincibilityTimer = 0.0f;
	float blinkTimer = 0.0f;
	bool isVisible = true;
	int Point = 0;
}

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
	transform_.position_ = XMFLOAT3(4, 0, 2);
	transform_.rotate_ = XMFLOAT3(0, 0, 0);
	//�R���C�_�[
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collider);
	Direction Front = FRONT;
	prevSpaceKey = false;
}

void Player::Update()
{
	//Direction currentDirection = GetDirectionFromInput();
	//transform_.rotate_.y = GetRotationFromDirection(currentDirection);
	if (Input::IsKeyDown(DIK_LEFT))
	{
		transform_.rotate_.y = 270;
	}
	if (Input::IsKeyDown(DIK_RIGHT))
	{
		transform_.rotate_.y = 90;
	}
	if (Input::IsKeyDown(DIK_UP))
	{
		transform_.rotate_.y = 0;
	}
	if (Input::IsKeyDown(DIK_DOWN))
	{
		transform_.rotate_.y = 180;
	}
	
	//�J����
	XMFLOAT3 camPos = transform_.position_;
	camPos.y = transform_.position_.y + 8.0f;
	camPos.z = transform_.position_.z - 5.0f;
	Camera::SetPosition(camPos);
	Camera::SetTarget(transform_.position_);

	//���G����
	if (isInvincible)
	{
		invincibilityTimer -= GetDeltaTime();
		blinkTimer -= GetDeltaTime();

		// �_�Ő���F0.1�b���Ƃɕ\���E��\����؂�ւ���
		if (blinkTimer <= 0.0f)
		{
			isVisible = !isVisible;
			blinkTimer = 0.1f; // �_�ŊԊu
		}

		if (invincibilityTimer <= 0.0f)
		{
			isInvincible = false;
			isVisible = true; // �ŏI�I�ɕ\���ɖ߂�
		}
	}
	else
	{
		isVisible = true; // ���G�łȂ���Ώ�ɕ\��
	}
}

void Player::Draw()
{
	if (!isVisible) return; // ��\����ԂȂ�`��X�L�b�v
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

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "RedEnemy")
	{
		HpDown(1);
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // ���������Ă����_�ŊJ�n
		if (HP <= 0)
		{
			this->KillMe();
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMEOVER);
		}
	}
	if (pTarget->GetObjectName() == "AllyBall")
	{
		PointUp(1);
		pTarget->KillMe();
		Debug::Log(Point);
		if (Point >= 1)
		{
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMECLEAR);
		}
	}
}

//Direction Player::GetDirectionFromInput()
//{
//	if (Input::IsKey(DIK_LEFT))  return LEFT;
//	if (Input::IsKey(DIK_RIGHT)) return RIGHT;
//	if (Input::IsKey(DIK_UP))    return FRONT;
//	if (Input::IsKey(DIK_DOWN))  return BACK;
//}
//
//int Player::GetRotationFromDirection(Direction dir)
//{
//	switch (dir)
//	{
//	case LEFT:  return 270;
//	case RIGHT: return 90;
//	case FRONT: return 0;
//	case BACK:  return 180;
//	}
//}

void Player::HpDown(int hp)
{
	HP -= hp;
}

float Player::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}

void Player::PointUp(int pt)
{
	Point += pt;
}
