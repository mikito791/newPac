#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"NeedleBall.h"
#include"Hp.h"
#include"HealBall.h"
#include"Bomb.h"
#include"Engine/Debug.h"
#include"Engine/Image.h"
#include"Stage.h"
#include"ReversalBall.h"
#include"CannonEnemy.h"
#include"Ghost.h"
#include"Engine/SceneManager.h"

namespace
{
	//�X�|�[���֌W
	float SpawnTimer = 0.0f;
	float spawnInterval = 30.0f; // �G��30�b���Ƃɏo��������
	float maxSpawnInterval = 30.0f; // �ő�̏o���Ԋu�i30�b�j
	const float minSpawnInterval = 3.0f;  // �ŒZ�̏o���Ԋu�i10�b�j
	float timeElapsed = 0.0f;  // �o�ߎ���
	//�ʒu
	XMFLOAT3 Left  (-2, 0, 2);
	XMFLOAT3 Right (10, 0, 2);
	XMFLOAT3 Back (4, 0, 6);
	XMFLOAT3 Front(4, 0, -4);
	
	// speed�����Ԍo�߂ő���������i�ő呬�x��ݒ�j
	const float maxSpeed = 0.2f;          // �ő呬�x�̗�
	const float speedIncrement = 0.005f; // 1�b�����葝�����鑬�x�̗�
	int frameCount = 0;           // �t���[���J�E���g�p
	const int framesPerBoost = 300; // ���t���[�����Ƃɑ��x���グ�邩�i��F300�t���[�� = 5�b��60fps�O��j
	//�G
	const int maxEnemyCount = 4; // �ő�G���i��F10�̂܂ŏo���\�j
	XMFLOAT3 EnemyLeft(-2.5, 0, 2);
	XMFLOAT3 EnemyRight(10.5, 0, 2);
	XMFLOAT3 EnemyBack(4, 0, 7);
	XMFLOAT3 EnemyFront(4, 0, -2);
	XMFLOAT3 rotLeft(0, 90, 0);
	XMFLOAT3 rotRight(0, 270, 0);
	XMFLOAT3 rotBack(0, 180, 0);
	XMFLOAT3 rotFront(0, 0, 0);
}

//�R���X�g���N�^
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),	speed(0.03f)
{
}

//������
void PlayScene::Initialize()
{
	//hPlayScene = Image::Load("Model//playScene.png");
	
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	Instantiate<Player>(this);
	Instantiate<RedWall>(this);
	Instantiate<CannonEnemy>(this);
	Instantiate<Stage>(this);
	Instantiate<Hp>(this);
	
	
}

//�X�V
void PlayScene::Update()
{
	float deltaTime = GetDeltaTime();
	SpawnTimer += deltaTime;
	timeElapsed += deltaTime;
	frameCount++;
	
	// speed���t���[�����Ƃɑ���
	if (frameCount % framesPerBoost == 0 && speed < maxSpeed)
	{
		speed += speedIncrement;
		if (speed > maxSpeed) speed = maxSpeed;
	}

	// �X�|�[���Ԋu�����ԁi�t���[���j�o�߂ɉ����ďk�߂�

	// �o���Ԋu�����Ԍo�߂ɍ��킹�Ē���
	if (timeElapsed >= maxSpawnInterval) 
	{
		// spawnInterval��i�K�I�ɏk�߂Ă���
		float timeFactor = (timeElapsed - maxSpawnInterval) / maxSpawnInterval;  // 30�b�ȍ~
		spawnInterval = max(minSpawnInterval, maxSpawnInterval - timeFactor * 27.0f);  // �ő�5�b�ɂȂ�悤�ɐݒ�
	}

	if (SpawnTimer >= spawnInterval)
	{
		//Update_CannonEnemy();
		SpawnTimer = 0.0f; // �^�C�}�[�����Z�b�g
		Update_SpawnNeedle();// ���{�[���̃X�|�[������
		if (rand() % 2 == 0) // 50%�̊m���Ŗ������X�|�[��
		{
			Update_SpawnHeal(); // �����̃X�|�[������
		}
		//Update_SpawnBomb();
		//Update_SpawnGhost(); // �S�[�X�g�̃X�|�[������
	}
	if (FindObject("CannonEnemy") == nullptr)
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMECLEAR); 
	}
}

//�`��
void PlayScene::Draw()
{
}

//�J��
void PlayScene::Release()
{
}

void PlayScene::Update_SpawnNeedle()
{
	NeedleRandom = rand() % 4; // 0�`3 �̃����_���l
	NeedleBall* nBall = nullptr; // NeedleBall�̃|�C���^
	switch (NeedleRandom)
	{
	case 0: // ������
		nBall = Instantiate<NeedleBall>(this);
		nBall->SetPos(Left);
		nBall->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // �E����
		nBall = Instantiate<NeedleBall>(this);
		nBall->SetPos(Right);
		nBall->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // ������
		nBall = Instantiate<NeedleBall>(this);
		nBall->SetPos(Back);
		nBall->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // ��O����
		nBall = Instantiate<NeedleBall>(this);
		nBall->SetPos(Front);
		nBall->SetMove(XMFLOAT3(0, 0, speed));
		break;
	default:
		break;
	}
}

void PlayScene::Update_SpawnHeal()
{
	HealBallRandom = rand() % 4; // 0�`3 �̃����_���l
	HealBall* hBall = nullptr; // AllyBall�̃|�C���^
	switch (HealBallRandom)
	{
	case 0: // ������
		hBall = Instantiate<HealBall>(this);
		hBall->SetPos(Left);
		hBall->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // �E����
		hBall = Instantiate<HealBall>(this);
		hBall->SetPos(Right);
		hBall->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // ������
		hBall = Instantiate<HealBall>(this);
		hBall->SetPos(Back);
		hBall->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // ��O����
		hBall = Instantiate<HealBall>(this);
		hBall->SetPos(Front);
		hBall->SetMove(XMFLOAT3(0, 0, speed));
		break;
	default:
		break;
	}
}

void PlayScene::Update_SpawnBomb()
{
	BombRandom = rand() % 4; // 0�`3 �̃����_���l
	Bomb* bomb = nullptr; // Bomb�̃|�C���^
	switch (BombRandom)
	{
	case 0: // ������
		bomb = Instantiate<Bomb>(this);
		bomb->SetPos(Left);
		bomb->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // �E����
		bomb = Instantiate<Bomb>(this);
		bomb->SetPos(Right);
		bomb->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // ������
		bomb = Instantiate<Bomb>(this);
		bomb->SetPos(Back);
		bomb->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // ��O����
		bomb = Instantiate<Bomb>(this);
		bomb->SetPos(Front);
		bomb->SetMove(XMFLOAT3(0, 0, speed));
		break;
	default:
		break;
	}
}

void PlayScene::Update_SpawnGhost()
{
	float GhostSpeed = 0.01f; // Ghost�̈ړ����x
	Ghost* ghost = nullptr; // Ghost�̃|�C���^
	GhostRandom = rand() % 4; // 0�`3 �̃����_���l
	switch (GhostRandom)
	{
	case 0: // ������
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Left);
		ghost->SetMove(XMFLOAT3(GhostSpeed, 0, 0));
		ghost->SetRot(XMFLOAT3(0, 90, 0)); // ������̌���
		break;
	case 1: // �E����
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Right);
		ghost->SetMove(XMFLOAT3(-GhostSpeed, 0, 0));
		ghost->SetRot(XMFLOAT3(0, 270, 0)); // �E����̌���
		break;
	case 2: // ������
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Back);
		ghost->SetMove(XMFLOAT3(0, 0, -GhostSpeed));
		ghost->SetRot(XMFLOAT3(0, 180, 0)); // ������̌���
		break;
	case 3: // ��O����
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Front);
		ghost->SetMove(XMFLOAT3(0, 0, GhostSpeed));
		ghost->SetRot(XMFLOAT3(0, 0, 0)); // ��O����̌���
		break;
	default:
		break;
	}
}

void PlayScene::Update_CannonEnemy()
{
	CannonEnemy* cEnemy = nullptr;
	for (int i = 0; i < EnemyNum; i++)
	{
		switch (i)
		{
		case 0: // ������
			Instantiate<CannonEnemy>(this);
			cEnemy->SetPos(EnemyLeft);
			cEnemy->SetRot(rotLeft);
			break;
		case 1: // �E����
			Instantiate<CannonEnemy>(this);
			cEnemy->SetPos(EnemyRight);
			cEnemy->SetRot(rotRight);
			break;
		case 2: // ������
			Instantiate<CannonEnemy>(this);
			cEnemy->SetPos(EnemyBack);
			cEnemy->SetRot(rotBack);
			break;
		case 3: // ��O����
			Instantiate<CannonEnemy>(this);
			cEnemy->SetPos(EnemyFront);
			cEnemy->SetRot(rotFront);
			break;
		default:
			break;
		}
	}
}

float PlayScene::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}