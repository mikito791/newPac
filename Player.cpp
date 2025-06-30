#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"Shield.h"
#include"NeedleBall.h"
#include"Hp.h"
#include"Engine/Debug.h"
#include"HealBall.h"
#include"Stage.h"
#include"ReversalBall.h"
#include"Engine/Audio.h"



namespace
{
	float invincibilityTime = 2.0f;
	bool isInvincible = false;
	float invincibilityTimer = 0.0f;
	float blinkTimer = 0.0f;
	bool isVisible = true;
	Direction Front = FRONT;
	float JumpHeight = 50.0f; // �W�����v�̍���
	float Gravity = 3.0f / 60.0f; // �d�͂̋���
	float MaxGravity = 6.0f; // �ő�d�͂̋���
	float correctionSpeed = 5.0f; // �␳�̑����i�����\�j

}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	hModel = -1;
	hDmageSound = -1;
	hHealSound = -1;
	hBombSound = -1;
	prevSpaceKey = false;
	HP = 50; // ����HP
	MaxHP = 50;
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
	csv.Load("CSV/variable.csv");
}

void Player::Update()
{
	float deltataTime = GetDeltaTime(); // �f���^�^�C�����擾
	// ReversalBall�ɓ��������瑀��𔽓]������

	Shield* pShield = (Shield*)FindObject("Shield");
	//���͏���
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection);

	//�ǂ̌����ƈʒu�����]������
	/*if (pRedWall)
	{
		pRedWall->SetDirection(reversedDirection);
		pRedWall->SetPosition(pReversalBall->GetPositionFromReveralDirection(reversedDirection));
	}*/


	//�J����
	XMFLOAT3 camPos = transform_.position_;
	camPos.y = transform_.position_.y + 8.0f;
	camPos.z = transform_.position_.z - 5.0f;
	Camera::SetPosition(camPos);
	Camera::SetTarget(transform_.position_);

	//���G����
	Invincible();
	//
	Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g��T��
	int hGroundModel = pStage->GetModelHandle();    //���f���ԍ����擾

	RayCastData data;
	data.start = transform_.position_;   //���C�̔��ˈʒu
	data.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
	Model::RayCast(hGroundModel, &data); //���C�𔭎�
	onGround = false;//�󒆂��f�t�H���g
	//���C������������
	if (data.hit)
	{
		float correction = data.dist * correctionSpeed * deltataTime; // �␳�l���v�Z
		//���̕��ʒu��������
		if (correction > data.dist)
		{
			correction = data.dist; // �␳�l�������𒴂��Ȃ��悤�ɐ���
		}
		transform_.position_.y -= correction;
		onGround = true; // �n�ʂɂ����Ԃɂ���
		jumpPower = 0.0f; // �W�����v�͂����Z�b�g

	}
	else
	{
		onGround = false; // �n�ʂɂ��Ȃ���Ԃɂ���
	}

	//�W�����v���
	if (Input::IsKeyDown(DIK_SPACE))
	{
		if (prevSpaceKey == false && onGround)
		{
			Jump();
		}
		prevSpaceKey = true;
	}
	else
	{
		prevSpaceKey = false; // �X�y�[�X�L�[�������ꂽ��t���O�����Z�b�g

	}

	if (!onGround)
	{
		// �󒆂ɂ���ꍇ�͏d�͂�K�p
		jumpPower += Gravity; // �d�͂�K�p
		if (jumpPower > MaxGravity) // �ő�d�͂𒴂��Ȃ��悤�ɐ���
		{
			jumpPower = MaxGravity;
		}
		transform_.position_.y += jumpPower; // �W�����v�̍������X�V
	}
	else
	{
		jumpPower = 0.0f; // �n�ʂɂ���ꍇ�̓W�����v�̗͂����Z�b�g
		onGround = true; // �n�ʂɂ����Ԃɂ���
	}

	//Debug::Log(transform_.position_.y,"\n"); 
	//if (Input::IsKey(DIK_K))
	//{
	//	transform_.position_.x += 0.1f; // J�L�[�ŏ㏸
	//}
	//if (Input::IsKey(DIK_L))
	//{
	//	transform_.position_.x -= 0.1f; // K�L�[�ŉ��~
	//}
	//if (Input::IsKey(DIK_I))
	//{
	//	transform_.position_.z += 0.1f; // J�L�[�ŏ㏸
	//}
	//if (Input::IsKey(DIK_O))
	//{
	//	transform_.position_.z -= 0.1f; // K�L�[�ŉ��~
	//}
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

float Player::CalculateDistance(const XMFLOAT3& PlayPos, const XMFLOAT3& Pos)
{
	//2�_�Ԃ̋������v�Z
	float dx = PlayPos.x - Pos.x;
	float dy = PlayPos.y - Pos.y;
	float dz = PlayPos.z - Pos.z;

	//������Ԃ�(�v�Z)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "NeedleBall")
	{
		HpDown(0.2);
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // ���������Ă����_�ŊJ�n
	}
	if (pTarget->GetObjectName() == "HealBall")
	{
		HpUp(5);
		pTarget->KillMe();
	}
	if (pTarget->GetObjectName() == "ReversalBall")
	{
		ReversalBall* pReversalBall = (ReversalBall*)FindObject("ReversalBall");
		Direction reversalDirection = pReversalBall->GetReveralDirectionFromInput();
		transform_.rotate_.y = pReversalBall->GetRotationFromReveralDirection(reversalDirection);
		//�ǂ̌����ƈʒu�����]������
	}
	if (pTarget->GetObjectName() == "Bomb")
	{
		HpDown(10);
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // ���������Ă����_�ŊJ�n
	}
	if (pTarget->GetObjectName() == "Ghost")
	{
		HpDown(0.05);
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // ���������Ă����_�ŊJ�n
	}
}

Direction Player::GetDirectionFromInput()
{
	static Direction lastDirection;

	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
	if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;
	/*if (OnReversal == false) {
		if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
		if (Input::IsKeyDown(DIK_RIGHT)|| Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
		if (Input::IsKeyDown(DIK_UP)   || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
		if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;
	}*/
	//else // ����𔽓]������
	//{
	//	if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = RIGHT; // �����͂�����ƉE������
	//	if (Input::IsKeyDown(DIK_RIGHT)|| Input::IsKeyDown(DIK_D)) lastDirection = LEFT;  // �E���͂�����ƍ�������
	//	if (Input::IsKeyDown(DIK_UP)   || Input::IsKeyDown(DIK_W)) lastDirection = BACK;  // ����͂�����ƌ�������
	//	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = FRONT; // �����͂�����ƑO������
	//}
	return lastDirection;
}

int Player::GetRotationFromDirection(Direction dir)
{
	int Left, Right, Front, Back;
	switch (dir)
	{
	case LEFT:
		return Left = csv.GetValue(1, 0);
		//return 270;
		break;
	case RIGHT:
		return Right = csv.GetValue(2, 0);
		//return 90;
		break;
	case FRONT:
		return Front = csv.GetValue(3, 0);
		//return 0;
		break;
	case BACK:
		return Back = csv.GetValue(4, 0);
		//return 180;
		break;
	default:
		break;
	}
}

void Player::HpDown(float hp)
{
	HP -= hp;
	if (HP <= 0)
	{
		HP = 0; // HP��0�����ɂȂ�Ȃ��悤�ɐ���
		this->KillMe();
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMEOVER);
	}
	Hp* pHp = (Hp*)(FindObject("Hp"));
	pHp->SetHpVal(HP, MaxHP); // HP�̍X�V
}

void Player::HpUp(int hp)
{
	HP += hp;
	if (HP > MaxHP)
	{
		HP = MaxHP; // HP���ő�l�𒴂��Ȃ��悤�ɐ���
	}
	Hp* pHp = (Hp*)(FindObject("Hp"));
	pHp->SetHpVal(HP, MaxHP); // HP�̍X�V
}

float Player::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}

void Player::Jump()
{
	jumpPower = sqrt(2.0f * JumpHeight * Gravity); // �W�����v�̏������v�Z
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
