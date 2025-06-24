#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"NeedleBall.h"
#include"Hp.h"
#include"AllyBall.h"
#include"Bomb.h"
#include"Engine/Debug.h"
#include"Engine/Image.h"
#include"Stage.h"
#include"ReversalBall.h"

namespace
{
	float SpawnTimer = 0.0f;
	float spawnInterval = 30.0f; // �G��60�b���Ƃɏo��������[1��]
	float maxSpawnInterval = 30.0f; // �ő�̏o���Ԋu�i30�b�j
	const float minSpawnInterval = 3.0f;  // �ŒZ�̏o���Ԋu�i10�b�j
	float timeElapsed = 0.0f;  // �o�ߎ���
	XMFLOAT3 Left  (-2, 0, 2);
	XMFLOAT3 Right (10, 0, 2);
	XMFLOAT3 Back (4, 0, 6);
	XMFLOAT3 Front(4, 0, -4);
	// speed�����Ԍo�߂ő���������i�ő呬�x��ݒ�j
	const float maxSpeed = 0.2f;          // �ő呬�x�̗�
	const float speedIncrement = 0.005f; // 1�b�����葝�����鑬�x�̗�
	int frameCount = 0;           // �t���[���J�E���g�p
	const int framesPerBoost = 300; // ���t���[�����Ƃɑ��x���グ�邩�i��F300�t���[�� = 5�b��60fps�O��j
}

//�R���X�g���N�^
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),	speed(0.05f)
{
}

//������
void PlayScene::Initialize()
{
	hPlayScene = Image::Load("Model//playScene.png");
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	Instantiate<Player>(this);
	Instantiate<RedWall>(this);
	//Instantiate<Hp>(this);
	Instantiate<Stage>(this);
	//Instantiate<ReversalBall>(this);
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
	if (timeElapsed >= maxSpawnInterval) // 30�b�o�ߌ�
	{
		// spawnInterval��i�K�I�ɏk�߂Ă���
		float timeFactor = (timeElapsed - maxSpawnInterval) / maxSpawnInterval;  // 30�b�ȍ~
		spawnInterval = max(minSpawnInterval, maxSpawnInterval - timeFactor * 27.0f);  // �ő�5�b�ɂȂ�悤�ɐݒ�
	}

	if (SpawnTimer >= spawnInterval)
	{
		
		SpawnTimer = 0.0f; // �^�C�}�[�����Z�b�g
		//Update_SpawnEnemy();// �G�̃X�|�[������
		//if (rand() % 2 == 0) // 50%�̊m���Ŗ������X�|�[��
		//{
		//	Update_SpawnAlly(); // �����̃X�|�[������
		//}
		ReversalBall* rBall = nullptr;
		rBall = Instantiate<ReversalBall>(this);
		rBall->SetPos(Left);
		rBall->SetMove(XMFLOAT3(speed, 0, 0));
		//Update_SpawnBomb();
	}
}

//�`��
void PlayScene::Draw()
{
	//Image::Draw(hPlayScene);
}

//�J��
void PlayScene::Release()
{
}

void PlayScene::Update_SpawnEnemy()
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

void PlayScene::Update_SpawnAlly()
{
	AllyRandom = rand() % 4; // 0�`3 �̃����_���l
	AllyBall* Aenemy = nullptr; // AllyBall�̃|�C���^
	switch (AllyRandom)
	{
	case 0: // ������
		Aenemy = Instantiate<AllyBall>(this);
		Aenemy->SetPos(Left);
		Aenemy->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // �E����
		Aenemy = Instantiate<AllyBall>(this);
		Aenemy->SetPos(Right);
		Aenemy->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // ������
		Aenemy = Instantiate<AllyBall>(this);
		Aenemy->SetPos(Back);
		Aenemy->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // ��O����
		Aenemy = Instantiate<AllyBall>(this);
		Aenemy->SetPos(Front);
		Aenemy->SetMove(XMFLOAT3(0, 0, speed));
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

float PlayScene::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}