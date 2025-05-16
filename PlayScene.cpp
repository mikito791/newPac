#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"RedEnemy.h"
#include"Hp.h"
#include"Engine/Debug.h"

namespace
{
	float enemySpawnTimer = 0.0f;
	float spawnInterval = 30.0f; // 敵を60秒ごとに出現させる[1分]
	float maxSpawnInterval = 30.0f; // 最大の出現間隔（30秒）
	const float minSpawnInterval = 3.0f;  // 最短の出現間隔（10秒）
	float timeElapsed = 0.0f;  // 経過時間
	XMFLOAT3 Left  (-2, 0, 2);
	XMFLOAT3 Right (10, 0, 2);
	XMFLOAT3 Back (4, 0, 6);
	XMFLOAT3 Front(4, 0, -4);
}

//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),	speed(0.05f)
{
}

//初期化
void PlayScene::Initialize()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
	Instantiate<Player>(this);
	Instantiate<RedWall>(this);
	//Instantiate<RedEnemy>(this);
	Instantiate<Hp>(this);
}

//更新
void PlayScene::Update()
{
	float deltaTime = GetDeltaTime();
	enemySpawnTimer += deltaTime;
	timeElapsed += deltaTime;
	
	// 出現間隔を時間経過に合わせて調整
	if (timeElapsed >= maxSpawnInterval) // 30秒経過後
	{
		// spawnIntervalを段階的に縮めていく
		float timeFactor = (timeElapsed - maxSpawnInterval) / maxSpawnInterval;  // 30秒以降
		spawnInterval = max(minSpawnInterval, maxSpawnInterval - timeFactor * 27.0f);  // 最大5秒になるように設定
	}

	if (enemySpawnTimer>=spawnInterval)
	{
		Debug::Log(enemySpawnTimer,"\n");
		enemySpawnTimer = 0.0f; // タイマーをリセット
		Debug::Log("敵が現れた\n");
		EnemyRandom = rand() % 4; // 0～3 のランダム値
		RedEnemy* Renemy = nullptr;
		switch (EnemyRandom)
		{
		case 0: // 左から
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Left);
			Renemy->SetMove(XMFLOAT3(speed, 0, 0));
			break;
		case 1: // 右から
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Right);
			Renemy->SetMove(XMFLOAT3(-speed, 0, 0));
			break;
		case 2: // 奥から
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Back);
			Renemy->SetMove(XMFLOAT3(0, 0, -speed));
			break;
		case 3: // 手前から
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Front);
			Renemy->SetMove(XMFLOAT3(0, 0, speed));
			break;
		default:
			break;
		}
	}
}

//描画
void PlayScene::Draw()
{
}

//開放
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