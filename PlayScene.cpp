#include "PlayScene.h"
#include"Player.h"
#include"Shield.h"
#include"NeedleBall.h"
#include"Hp.h"
#include"HealBall.h"
#include"Bomb.h"
#include"Engine/Debug.h"
#include"Engine/Image.h"
#include"Stage.h"
#include"ReversalBall.h"
#include"Ghost.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

namespace
{
	//�X�|�[���֌W
	float SpawnTimer = 0.0f;
	float spawnInterval = 30.0f; // �G��30�b���Ƃɏo��������
	float maxSpawnInterval = 30.0f; // �ő�̏o���Ԋu�i30�b�j
	const float minSpawnInterval = 3.0f;  // �ŒZ�̏o���Ԋu�i10�b�j
	float timeElapsed = 0.0f;  // �o�ߎ���
	//�ʒu
	XMFLOAT3 Left(-2, 0, 2);
	XMFLOAT3 Right(10, 0, 2);
	XMFLOAT3 Back(4, 0, 6);
	XMFLOAT3 Front(4, 0, -1.5);

	// speed�����Ԍo�߂ő���������i�ő呬�x��ݒ�j
	const float maxSpeed = 0.2f;          // �ő呬�x�̗�
	const float speedIncrement = 0.005f; // 1�b�����葝�����鑬�x�̗�
	int frameCount = 0;           // �t���[���J�E���g�p
	const int framesPerBoost = 300; // ���t���[�����Ƃɑ��x���グ�邩�i��F300�t���[�� = 5�b��60fps�O��j
	//�G
	const int maxEnemyCount = 1; // �ő�G���i��F4�̂܂ŏo���\�j����3�E�̓G���̗͖�����ԂɂȂ��Ă�
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
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
	
	hPlayScene = -1; // ���f���n���h���̏�����
	speed = 0.03f; // �����̈ړ����x��ݒ�
	SpawnTimer = 0.0f; // �X�|�[���^�C�}�[�̏�����
	spawnInterval = 30.0f; // �����̃X�|�[���Ԋu��30�b�ɐݒ�
	timeElapsed = 0.0f; // �o�ߎ��Ԃ̏�����
	frameCount = 0; // �t���[���J�E���g�̏�����
	hBGM = -1; // BGM�̃n���h��������
}

//������
void PlayScene::Initialize()
{
	//hPlayScene = Image::Load("Model//playScene.png");
	csv.Load("CSV/Enemy.csv"); // CSV�t�@�C���̓ǂݍ���
	hBGM = Audio::Load("Sound/StageBGM.wav"); // BGM�̓ǂݍ���
	assert(hBGM >= 0);
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	Instantiate<Player>(this);
	Instantiate<Shield>(this);
	Instantiate<Stage>(this);
	Instantiate<Hp>(this);
	Update_CannonEnemy();
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

		SpawnTimer = 0.0f; // �^�C�}�[�����Z�b�g
		
		if (rand() % 2 == 0) // 50%�̊m���Ŗ������X�|�[��
		{
			Update_SpawnBomb(); //���e�̃X�|�[������
			Update_SpawnHeal(); // �����̃X�|�[������
		}
		else
		{
			Update_SpawnNeedle();// ���{�[���̃X�|�[������
			Update_SpawnGhost(); // �S�[�X�g�̃X�|�[������
		}
		if (rand() % 5 == 0) // 1/5�̊m���Ŕ��]�{�[�����X�|�[��
		{
			Update_SpawnReversalBall(); // ���]�{�[���̃X�|�[������
		}
	}
	if (FindObject("CannonEnemy") == nullptr)
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMECLEAR);
	}
	Audio::Play(hBGM); // BGM���Đ�
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
	int NeedleRandom = rand() % 4; // 0�`3 �̃����_���l
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
	int HealBallRandom = rand() % 4; // 0�`3 �̃����_���l
	HealBall* hBall = nullptr; // HealBall�̃|�C���^
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
	int BombRandom = rand() % 4; // 0�`3 �̃����_���l
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

void PlayScene::Update_SpawnReversalBall()
{
	int ReversalBallRandom = rand() % 4; // 0�`3 �̃����_���l
	ReversalBall* rBall = nullptr; // ReversalBall�̃|�C���^
	switch (ReversalBallRandom)
	{
	case 0: // ������
		rBall = Instantiate<ReversalBall>(this);
		rBall->SetPos(Left);
		rBall->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // �E����
		rBall = Instantiate<ReversalBall>(this);
		rBall->SetPos(Right);
		rBall->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // ������
		rBall = Instantiate<ReversalBall>(this);
		rBall->SetPos(Back);
		rBall->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // ��O����
		rBall = Instantiate<ReversalBall>(this);
		rBall->SetPos(Front);
		rBall->SetMove(XMFLOAT3(0, 0, speed));
		break;
	default:
		break;
	}
}

void PlayScene::Update_SpawnGhost()
{
	float GhostSpeed = 0.02f; // Ghost�̈ړ����x
	Ghost* ghost = nullptr; // Ghost�̃|�C���^
	int GhostRandom = rand() % 4; // 0�`3 �̃����_���l
	switch (GhostRandom)
	{
	case 0: // ������
		
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Left);
		ghost->SetMove(XMFLOAT3(GhostSpeed, 0, 0));
		ghost->SetRot(rotLeft); // ������̌���
		break;
	case 1: // �E����
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Right);
		ghost->SetMove(XMFLOAT3(-GhostSpeed, 0, 0));
		ghost->SetRot(rotRight); // �E����̌���
		break;
	case 2: // ������
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Back);
		ghost->SetMove(XMFLOAT3(0, 0, -GhostSpeed));
		ghost->SetRot(rotBack); // ������̌���
		break;
	case 3: // ��O����
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Front);
		ghost->SetMove(XMFLOAT3(0, 0, GhostSpeed));
		ghost->SetRot(rotFront); // ��O����̌���
		break;
	default:
		break;
	}
}

void PlayScene::Update_CannonEnemy()
{
	XMFLOAT3 positions[] = { EnemyLeft, EnemyBack,EnemyFront,EnemyRight};
	XMFLOAT3 rotations[] = { rotLeft, rotBack,rotFront,rotRight, };

	enemies.clear(); // �����̓G���N���A

	for (int i = 0; i < maxEnemyCount; ++i)
	{
		CannonEnemy* enemy = Instantiate<CannonEnemy>(this);
		enemy->SetPos(positions[i]);
		enemy->SetRot(rotations[i]);
		enemies.push_back(enemy);
	}
}

float PlayScene::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}