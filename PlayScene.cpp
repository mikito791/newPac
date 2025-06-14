#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"RedEnemy.h"
#include"Hp.h"
#include"AllyBall.h"
#include"Engine/Debug.h"
#include"Engine/Image.h"
#include"Stage.h"

namespace
{
	float SpawnTimer = 0.0f;
	float spawnInterval = 30.0f; // 敵を60秒ごとに出現させる[1分]
	float maxSpawnInterval = 30.0f; // 最大の出現間隔（30秒）
	const float minSpawnInterval = 3.0f;  // 最短の出現間隔（10秒）
	float timeElapsed = 0.0f;  // 経過時間
	XMFLOAT3 Left  (-2, 0, 2);
	XMFLOAT3 Right (10, 0, 2);
	XMFLOAT3 Back (4, 0, 6);
	XMFLOAT3 Front(4, 0, -4);
	// speedを時間経過で増加させる（最大速度を設定）
	const float maxSpeed = 0.2f;          // 最大速度の例
	const float speedIncrement = 0.005f; // 1秒あたり増加する速度の例
	int frameCount = 0;           // フレームカウント用
	const int framesPerBoost = 300; // 何フレームごとに速度を上げるか（例：300フレーム = 5秒間60fps前提）
}

//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),	speed(0.05f)
{
}

//初期化
void PlayScene::Initialize()
{
	hPlayScene = Image::Load("Model//playScene.png");
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
	Instantiate<Player>(this);
	Instantiate<RedWall>(this);
	//Instantiate<Hp>(this);
	Instantiate<Stage>(this);
}

//更新
void PlayScene::Update()
{
	float deltaTime = GetDeltaTime();
	SpawnTimer += deltaTime;
	timeElapsed += deltaTime;
	frameCount++;

	// speedをフレームごとに増加
	if (frameCount % framesPerBoost == 0 && speed < maxSpeed)
	{
		speed += speedIncrement;
		if (speed > maxSpeed) speed = maxSpeed;
	}

	// スポーン間隔を時間（フレーム）経過に応じて縮める

	// 出現間隔を時間経過に合わせて調整
	if (timeElapsed >= maxSpawnInterval) // 30秒経過後
	{
		// spawnIntervalを段階的に縮めていく
		float timeFactor = (timeElapsed - maxSpawnInterval) / maxSpawnInterval;  // 30秒以降
		spawnInterval = max(minSpawnInterval, maxSpawnInterval - timeFactor * 27.0f);  // 最大5秒になるように設定
	}

	if (SpawnTimer >= spawnInterval)
	{
		
		SpawnTimer = 0.0f; // タイマーをリセット
		Update_Spawn();
	}
}

//描画
void PlayScene::Draw()
{
	//Image::Draw(hPlayScene);
}

//開放
void PlayScene::Release()
{
}

void PlayScene::Update_Spawn()
{
	EnemyRandom = rand() % 4; // 0〜3 のランダム値
	AllyRandom = rand() % 4; // 0〜3 のランダム値
	RedEnemy* Renemy = nullptr; // RedEnemyのポインタ
	AllyBall* Aenemy = nullptr; // AllyBallのポインタ
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
	switch (AllyRandom)
	{
	case 0: // 左から
		Aenemy = Instantiate<AllyBall>(this);
		Aenemy->SetPos(Left);
		Aenemy->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // 右から
		Aenemy = Instantiate<AllyBall>(this);
		Aenemy->SetPos(Right);
		Aenemy->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // 奥から
		Aenemy = Instantiate<AllyBall>(this);
		Aenemy->SetPos(Back);
		Aenemy->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // 手前から
		Aenemy = Instantiate<AllyBall>(this);
		Aenemy->SetPos(Front);
		Aenemy->SetMove(XMFLOAT3(0, 0, speed));
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