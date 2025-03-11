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
    void OnCollision(GameObject* pTarget) override; // OnCollisionメソッドの宣言

private:
    void SpawnEnemy();
    bool ShouldSpawn();
    float GetDeltaTime();

    std::vector<RedEnemy*> enemies;
    float spawnInterval; // スポーン間隔（秒）
    float timeSinceLastSpawn; // 最後のスポーンからの経過時間
    std::chrono::steady_clock::time_point lastUpdateTime; // 最後の更新時間
};
