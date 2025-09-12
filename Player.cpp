#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Camera.h"
#include"Engine/Audio.h"
#include"Engine/Debug.h"

#include"NeedleBall.h"
#include"Hp.h"
#include"HealBall.h"
#include"Stage.h"
#include"ReversalBall.h"



namespace
{
	bool isBlinking = false;         // �_���[�W���o���i�_�Œ��j���ǂ���
	float blinkDuration = 1.0f;      // �_�őS�̂̒����i1�b�Ȃǁj
	float blinkTimer = 0.0f;         // �c��_�Ŏ���
	float blinkInterval = 0.1f;      // �_�ł�ON/OFF�̊Ԋu
	float blinkToggleTimer = 0.0f;   // ���ɐ؂�ւ���܂ł̎���

	bool isVisible = true;           // ���ݕ\������Ă��邩
	Direction Front = FRONT;
	const float reversalDuration = 5.0f; // ���]�̎������ԁi�b�j
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	hPlayer = -1;
	prevSpaceKey = true;
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
	//
	pFlashLight = new FlashLight(this);
	pShield = new Shield(this);	
	pFlashLight->Initialize();
	pShield->Initialize();
}

void Player::Update()
{
	float deltaTime = GetDeltaTime(); // �f���^�^�C�����擾
	
	//���͏���
	Direction currentDirection = GetDirectionFromInput();
	transform_.rotate_.y = GetRotationFromDirection(currentDirection);
	//���̈ʒu�ƌ������X�V
	ShieldTrans.position_ = GetPositionFromDirection(currentDirection);
	ShieldPos = ShieldTrans.position_;
	ShieldTrans.rotate_.y = GetRotationFromDirection(currentDirection);
	ShieldRotY = ShieldTrans.rotate_.y;
	//�����d���̈ʒu�ƌ������X�V
	FlashLightTrans.position_ = GetPositionFromDirection(currentDirection);
	FlashLightPos = FlashLightTrans.position_;
	FlashLightTrans.rotate_.y = GetRotationFromDirection(currentDirection);
	FlashLightRotY = FlashLightTrans.rotate_.y;

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
	
	//���C������������
	if (data.hit)
	{
		transform_.position_.y -= data.dist;
	}
	
	if (Input::IsKeyDown(DIK_SPACE))
	{
		//���Ɖ����d����؂�ւ���
		prevSpaceKey = !prevSpaceKey;
	}
	if (prevSpaceKey)
	{
		//pShield->SetTransform(ShieldPos, ShieldRotY);
		pShield->Update();
	}
	else
	{
		//pFlashLight->SetTransform(FlashLightPos, FlashLightRotY);
		pFlashLight->Update();
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
	if (prevSpaceKey)
	{
		pShield->SetTransform(ShieldPos, ShieldRotY);
		pShield->Draw();
	}
	else
	{
		pFlashLight->SetTransform(FlashLightPos, FlashLightRotY);
		pFlashLight->Draw();
	}
	if (!isVisible) return; // ��\����ԂȂ�`��X�L�b�v
	Model::SetTransform(hPlayer, transform_);
	Model::Draw(hPlayer);

	if (!IsConfusion)return;
	Model::SetTransform(hConfusion, ConTrans);
	Model::Draw(hConfusion); // �����G�t�F�N�g�̕`��
}

void Player::Release()
{
	if (hPlayer >= 0)
	{
		Model::Release(hPlayer);
		hPlayer = -1;
	}
	if (hConfusion >= 0)
	{
		Model::Release(hConfusion); // �����G�t�F�N�g�̃��f�������
		hConfusion = -1;
	}
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "NeedleBall")
	{
		HpDown(0.5);
		// �_�ŊJ�n
		isBlinking = true;
		blinkTimer = blinkDuration;
		blinkToggleTimer = 0.0f;
		isVisible = false;               // �ŏ��͔�\������n�߂�
	}
	if (pTarget->GetObjectName() == "HealBall")
	{
		HpUp(5);
		pTarget->KillMe();
		Audio::Play(hHealSound); // �q�[�������Đ�
	}
	if (pTarget->GetObjectName() == "ReversalBall")
	{
		pTarget->KillMe(); // ReversalBall�ɓ��������玩�����폜
		//player�̑���𔽓]
		ReversalBall* pReversalBall = dynamic_cast<ReversalBall*>(pTarget);
		Direction reversalDirection = pReversalBall->GetReveralDirectionFromInput();
		transform_.rotate_.y = pReversalBall->GetRotationFromReveralDirection(reversalDirection);
		//Shield�ɔ��]��ʒm
		Shield* pShield = (Shield*)FindObject("Shield");
		if (pShield)
		{
			pShield->StartReversalShield();
		}
		//FlashLight�ɔ��]��ʒm
		FlashLight* pFlashLight = (FlashLight*)FindObject("FlashLight");
		if (pFlashLight)
		{
			pFlashLight->StartReversalLight();
		}
		//���]��Ԃɂ���
		onReversal = true;
		reversalTimer = 0.0f;
		IsConfusion = true; // �����G�t�F�N�gON
	}
	if (pTarget->GetObjectName() == "Bomb")
	{
		HpDown(10);
		// �_�ŊJ�n
		isBlinking = true;
		blinkTimer = blinkDuration;
		blinkToggleTimer = 0.0f;
		isVisible = false;               // �ŏ��͔�\������n�߂�
	}
	if (pTarget->GetObjectName() == "Ghost")
	{
		HpDown(0.05);
		Audio::Play(hDamageSound); // �_���[�W�����Đ�
		// �_�ŊJ�n
		isBlinking = true;
		blinkTimer = blinkDuration;
		blinkToggleTimer = 0.0f;
		isVisible = false;               // �ŏ��͔�\������n�߂�
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

XMFLOAT3 Player::GetPositionFromDirection(Direction dir)
{
	//csv�ňʒu���擾�ł���悤�ɂ���
	float x, y, z;
	switch (dir)
	{
	case LEFT:
		x = csv.GetValue(1, 1);
		y = csv.GetValue(2, 1);
		z = csv.GetValue(3, 1);
		return XMFLOAT3(x, y, z);
		break;
	case RIGHT:
		x = csv.GetValue(4, 1);
		y = csv.GetValue(2, 1);
		z = csv.GetValue(3, 1);
		return XMFLOAT3(x, y, z);
		break;
	case FRONT:
		x = csv.GetValue(1, 2);
		y = csv.GetValue(2, 2);
		z = csv.GetValue(3, 2);
		return XMFLOAT3(x, y, z);
		break;
	case BACK:
		x = csv.GetValue(1, 2);
		y = csv.GetValue(2, 2);
		z = csv.GetValue(4, 2);
		return XMFLOAT3(x, y, z);
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

void Player::Invincible()
{
	if (isBlinking)
	{
		blinkTimer -= GetDeltaTime();
		blinkToggleTimer -= GetDeltaTime();

		// �_�Ő؂�ւ�
		if (blinkToggleTimer <= 0.0f)
		{
			isVisible = !isVisible;
			blinkToggleTimer = blinkInterval;
		}

		// �_�ŏI��
		if (blinkTimer <= 0.0f)
		{
			isBlinking = false;
			isVisible = true;  // �Ō�ɕ\���ŏI��
		}
	}
}
