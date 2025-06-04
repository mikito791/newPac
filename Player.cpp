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
	float invincibilityTime = 2.0f;
	bool isInvincible = false;
	float invincibilityTimer = 0.0f;
	float blinkTimer = 0.0f;
	bool isVisible = true;
	Direction Front = FRONT;
	float JumpHeight = 0.1f; // �W�����v�̍���
	float Gravity = 9.8f/60.0f; // �d�͂̋���
	float MaxGravity = 6.0f; // �ő�d�͂̋���
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	hModel = -1;
	jumpPower = 0.0f; // �W�����v�̗�

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
	transform_.rotate_.y = GetRotationFromDirection(Front); // ����������O�ɐݒ�
	prevSpaceKey = false;
	HP = 50;
	Point = 0;
}

void Player::Update()
{
	//���͏���
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection);
	
	//�J����
	XMFLOAT3 camPos = transform_.position_;
	camPos.y = transform_.position_.y + 8.0f;
	camPos.z = transform_.position_.z - 5.0f;
	Camera::SetPosition(camPos);
	Camera::SetTarget(transform_.position_);

	//���G����
	Invincible();
	//�W�����v���
	if (Input::IsKeyDown(DIK_SPACE))
	{
		if (prevSpaceKey == false)
		{
			if (onGround)
			{
				Jump();
			}
		}
		prevSpaceKey = true; 
	}
	else
	{
		prevSpaceKey = false; // �X�y�[�X�L�[�������ꂽ��t���O�����Z�b�g
	}
	jumpPower += Gravity; // �d�͂�K�p
	if (jumpPower > MaxGravity)
	{
		jumpPower = MaxGravity; // �ő�d�͂𒴂��Ȃ��悤�ɐ���
	}
	transform_.position_.y += jumpPower; // �W�����v�̍������X�V
	if (transform_.position_.y <= 0.0f) // �n�ʂɒ��n������
	{
		transform_.position_.y = 0.0f; // �n�ʂ̍����ɍ��킹��
		onGround = true; // �n�ʂɂ����Ԃɂ���
		jumpPower = 0.0f; // �W�����v�͂����Z�b�g
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
		if (Point >= 5)
		{
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMECLEAR);
		}
	}
}

Direction Player::GetDirectionFromInput()
{
	static Direction lastDirection = FRONT;

	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
	if (Input::IsKeyDown(DIK_RIGHT)|| Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
	if (Input::IsKeyDown(DIK_UP)   || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;

	return lastDirection;
}

int Player::GetRotationFromDirection(Direction dir)
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

void Player::Jump()
{
	jumpPower = -sqrt(2.0f * JumpHeight * Gravity); // �W�����v�̏������v�Z
	onGround = false; // �W�����v���͒n�ʂɂ��Ȃ��Ƃ���
}

void Player::Invincible()
{
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
