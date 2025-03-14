#include "Spawner.h"
#include "RedEnemy.h"
#include"Engine/SphereCollider.h"
#include <vector>

Spawner::Spawner(GameObject* parent)
    : GameObject(parent, "Spawner"), spawnInterval(5.0f), timeSinceLastSpawn(0.0f) // スポーン間隔を5秒に設定
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
    // 必要に応じて初期化コードを追加
}

void Spawner::Update()
{
    // 経過時間を更新
    timeSinceLastSpawn += GetDeltaTime(); // GetDeltaTime()はフレーム間の経過時間を返す関数と仮定

    // スポーン条件をチェック
    if (ShouldSpawn())
    {
        SpawnEnemy();
        timeSinceLastSpawn = 0.0f; // スポーン後にタイマーをリセット
    }

    // 各敵の更新
    for (auto enemy : enemies)
    {
        enemy->Update();
    }
}

void Spawner::Draw()
{
    // 各敵の描画
    for (auto enemy : enemies)
    {
        enemy->Draw();
    }
}

void Spawner::Release()
{
    // 必要に応じてリリースコードを追加
}

void Spawner::SpawnEnemy()
{
    RedEnemy* newEnemy = new RedEnemy(this);
    newEnemy->Initialize();
    enemies.push_back(newEnemy);

    SphereCollider* collision = new SphereCollider(newEnemy->GetTransform().position_, 0.5f); // 半径0.5のコライダーを設定
    newEnemy->AddCollider(collision);
}

bool Spawner::ShouldSpawn()
{
    // スポーン間隔を超えた場合にスポーン
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