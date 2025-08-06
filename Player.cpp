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
	const float reversalDuration = 5.0f; // ���]�̎������ԁi�b�j
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	hPlayer = -1;
	prevSpaceKey = false;
	HP = 50; // ����HP
	MaxHP = 50;
	hHealSound = -1; // �q�[�����̃n���h��
	hDamageSound = -1; // �_���[�W���̃n���h��
}

Player::~Player()
{
}

void Player::Initialize()
{
	hPlayer = Model::Load("Model/Player.fbx");
	assert(hPlayer >= 0);
	hConfusion = Model::Load("Model/Confusion.fbx"); // �����G�t�F�N�g�̃��f���ǂݍ���
	assert(hConfusion >= 0);
	hHealSound = Audio::Load("Sound/Heal.wav"); // �q�[�����̓ǂݍ���
	assert(hHealSound >= 0);
	hDamageSound = Audio::Load("Sound/Damage.wav"); // �_���[�W���̓ǂݍ���
	assert(hDamageSound >= 0);
	//transform_.position_.y = 10;
	transform_.position_ = XMFLOAT3(4, 0, 2);
	transform_.rotate_ = XMFLOAT3(0, 0, 0);
	//�R���C�_�[
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collider);
	csv.Load("CSV/Player.csv");
	onReversal = false;
	reversalTimer = 0.0f; // ���]�^�C�}�[�̏�����
}

void Player::Update()
{
	float deltaTime = GetDeltaTime(); // �f���^�^�C�����擾
	
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
		float correction = data.dist * correctionSpeed * deltaTime; // �␳�l���v�Z
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

	//���씽�]
	if (onReversal)
	{
		reversalTimer += deltaTime;
		if (reversalTimer >= reversalDuration)
		{
			onReversal = false;
			reversalTimer = 0.0f;
			IsConfusion = false; // ���]���I�������獬����Ԃ�����
		}
	}

	//�����G�t�F�N�g����
	ConTrans.position_ = transform_.position_;
	ConTrans.rotate_.y += 3; // �v���C���[�̌����ɍ��킹��

}

void Player::Draw()
{	
	if (!isVisible) return; // ��\����ԂȂ�`��X�L�b�v
	Model::SetTransform(hPlayer, transform_);
	Model::Draw(hPlayer);

	if (!IsConfusion)return;
	Model::SetTransform(hConfusion, ConTrans);
	Model::Draw(hConfusion); // �����G�t�F�N�g�̕`��
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
		HpDown(0.5);
		Audio::Play(hDamageSound); // �_���[�W�����Đ�
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // ���������Ă����_�ŊJ�n
	}
	if (pTarget->GetObjectName() == "HealBall")
	{
		HpUp(5);
		pTarget->KillMe();
		Audio::Play(hHealSound); // �q�[�������Đ�
	}
	if (pTarget->GetObjectName() == "ReversalBall")
	{
		//1:ReversalBall�ɓ��������瑀��𔽓]������
		pTarget->KillMe(); // ReversalBall�ɓ��������玩�����폜
		ReversalBall* pReversalBall = dynamic_cast<ReversalBall*>(pTarget);
		Direction reversalDirection = pReversalBall->GetReveralDirectionFromInput();
		transform_.rotate_.y = pReversalBall->GetRotationFromReveralDirection(reversalDirection);
		//Shield�ɔ��]��ʒm
		Shield* pShield = (Shield*)FindObject("Shield");
		if (pShield)
		{
			pShield->StartReversal();
		}
		onReversal = true;
		reversalTimer = 0.0f;
		IsConfusion = true; // ������Ԃɂ���
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
		Audio::Play(hDamageSound); // �_���[�W�����Đ�
		isInvincible = true;
		invincibilityTimer = invincibilityTime;
		blinkTimer = 0.0f; // ���������Ă����_�ŊJ�n
	}
}

Direction Player::GetDirectionFromInput() const
{
	static Direction lastDirection;

	/*if (Input::IsKeyDown(DIK_LEFT) || Input::IsKeyDown(DIK_A)) lastDirection = LEFT;
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsKeyDown(DIK_D)) lastDirection = RIGHT;
	if (Input::IsKeyDown(DIK_UP) || Input::IsKeyDown(DIK_W)) lastDirection = FRONT;
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsKeyDown(DIK_S)) lastDirection = BACK;*/
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
