#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"RedEnemy.h"
#include"Hp.h"
#include"Engine/Debug.h"

namespace
{
	float enemySpawnTimer = 0.0f;
	float spawnInterval = 30.0f; // �G��60�b���Ƃɏo��������[1��]
	float maxSpawnInterval = 30.0f; // �ő�̏o���Ԋu�i30�b�j
	const float minSpawnInterval = 3.0f;  // �ŒZ�̏o���Ԋu�i10�b�j
	float timeElapsed = 0.0f;  // �o�ߎ���
	XMFLOAT3 Left  (-2, 0, 2);
	XMFLOAT3 Right (10, 0, 2);
	XMFLOAT3 Back (4, 0, 6);
	XMFLOAT3 Front(4, 0, -4);
}

//�R���X�g���N�^
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),	speed(0.05f)
{
}

//������
void PlayScene::Initialize()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // �����������i����Ⴄ���ʂɂ���j
	Instantiate<Player>(this);
	Instantiate<RedWall>(this);
	//Instantiate<RedEnemy>(this);
	Instantiate<Hp>(this);
}

//�X�V
void PlayScene::Update()
{
	float deltaTime = GetDeltaTime();
	enemySpawnTimer += deltaTime;
	timeElapsed += deltaTime;
	
	// �o���Ԋu�����Ԍo�߂ɍ��킹�Ē���
	if (timeElapsed >= maxSpawnInterval) // 30�b�o�ߌ�
	{
		// spawnInterval��i�K�I�ɏk�߂Ă���
		float timeFactor = (timeElapsed - maxSpawnInterval) / maxSpawnInterval;  // 30�b�ȍ~
		spawnInterval = max(minSpawnInterval, maxSpawnInterval - timeFactor * 27.0f);  // �ő�5�b�ɂȂ�悤�ɐݒ�
	}

	if (enemySpawnTimer>=spawnInterval)
	{
		Debug::Log(enemySpawnTimer,"\n");
		enemySpawnTimer = 0.0f; // �^�C�}�[�����Z�b�g
		Debug::Log("�G�����ꂽ\n");
		EnemyRandom = rand() % 4; // 0�`3 �̃����_���l
		RedEnemy* Renemy = nullptr;
		switch (EnemyRandom)
		{
		case 0: // ������
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Left);
			Renemy->SetMove(XMFLOAT3(speed, 0, 0));
			break;
		case 1: // �E����
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Right);
			Renemy->SetMove(XMFLOAT3(-speed, 0, 0));
			break;
		case 2: // ������
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Back);
			Renemy->SetMove(XMFLOAT3(0, 0, -speed));
			break;
		case 3: // ��O����
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Front);
			Renemy->SetMove(XMFLOAT3(0, 0, speed));
			break;
		default:
			break;
		}
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

float PlayScene::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}