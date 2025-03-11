#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include <chrono>

class RedEnemy;

class Spawner : public GameObject
{
public:
    Spawner(GameObject* parent);
    ~Spawner();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnCollision(GameObject* pTarget) override; // OnCollision���\�b�h�̐錾

private:
    void SpawnEnemy();
    bool ShouldSpawn();
    float GetDeltaTime();

    std::vector<RedEnemy*> enemies;
    float spawnInterval; // �X�|�[���Ԋu�i�b�j
    float timeSinceLastSpawn; // �Ō�̃X�|�[������̌o�ߎ���
    std::chrono::steady_clock::time_point lastUpdateTime; // �Ō�̍X�V����
};
