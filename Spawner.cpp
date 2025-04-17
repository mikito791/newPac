#include "Spawner.h"
#include "RedEnemy.h"
#include"Engine/SphereCollider.h"
#include <vector>

Spawner::Spawner(GameObject* parent)
    : GameObject(parent, "Spawner"), spawnInterval(5.0f), timeSinceLastSpawn(0.0f) // �X�|�[���Ԋu��5�b�ɐݒ�
{
}

Spawner::~Spawner()
{
    for (auto enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}

void Spawner::Initialize()
{
    // �K�v�ɉ����ď������R�[�h��ǉ�
    //SpawnEnemy();
}

void Spawner::Update()
{
    // �o�ߎ��Ԃ��X�V
    timeSinceLastSpawn += GetDeltaTime(); // GetDeltaTime()�̓t���[���Ԃ̌o�ߎ��Ԃ�Ԃ��֐��Ɖ���

    // �X�|�[���������`�F�b�N
    if (ShouldSpawn())
    {
        SpawnEnemy();
        timeSinceLastSpawn = 0.0f; // �X�|�[����Ƀ^�C�}�[�����Z�b�g
    }

    // �e�G�̍X�V
    for (auto enemy : enemies)
    {
        enemy->Update();
    }
}

void Spawner::Draw()
{
    // �e�G�̕`��
    for (auto enemy : enemies)
    {
        enemy->Draw();
    }
}

void Spawner::Release()
{
	// �e�G�̉��
    for (auto enemy : enemies)
    {
		enemy->Release();
	}
	// enemies�̃|�C���^�����
    for (auto enemy : enemies)
    {
		delete enemy;
	}
	enemies.clear();
}
// �K�v�ɉ����ă����[�X�R�[�h��ǉ�


void Spawner::SpawnEnemy()
{
    RedEnemy* newEnemy =(RedEnemy*)FindObject("RedEnemy");
    //XMFLOAT3 pos=XMFLOAT3(0, 0, 0); // �X�|�[���ʒu���w��i��F���_�j
    //XMFLOAT3 enepos = newEnemy->GetPos(); // �V�����G�̈ʒu���擾
    ////pos = enepos;
    enemies.push_back(newEnemy);
}

bool Spawner::ShouldSpawn()
{
    // �X�|�[���Ԋu�𒴂����ꍇ�ɃX�|�[��
    return timeSinceLastSpawn >= spawnInterval;
}

float Spawner::GetDeltaTime()
{
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    return deltaTime.count();
}

void Spawner::OnCollision(GameObject* pTarget)
{
    for (auto enemy : enemies)
    {
        if (pTarget->GetObjectName() == "RedWall")
        {
            enemy->KillMe();
        }
    }
}