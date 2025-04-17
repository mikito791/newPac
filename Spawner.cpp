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
    //SpawnEnemy();
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
	// 各敵の解放
    for (auto enemy : enemies)
    {
		enemy->Release();
	}
	// enemiesのポインタを解放
    for (auto enemy : enemies)
    {
		delete enemy;
	}
	enemies.clear();
}
// 必要に応じてリリースコードを追加


void Spawner::SpawnEnemy()
{
    RedEnemy* newEnemy =(RedEnemy*)FindObject("RedEnemy");
    //XMFLOAT3 pos=XMFLOAT3(0, 0, 0); // スポーン位置を指定（例：原点）
    //XMFLOAT3 enepos = newEnemy->GetPos(); // 新しい敵の位置を取得
    ////pos = enepos;
    enemies.push_back(newEnemy);
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