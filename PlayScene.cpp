#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"NeedleBall.h"
#include"Hp.h"
#include"HealBall.h"
#include"Bomb.h"
#include"Engine/Debug.h"
#include"Engine/Image.h"
#include"Stage.h"
#include"ReversalBall.h"
#include"CannonEnemy.h"
#include"Ghost.h"
#include"Engine/SceneManager.h"

namespace
{
	//スポーン関係
	float SpawnTimer = 0.0f;
	float spawnInterval = 30.0f; // 敵を30秒ごとに出現させる
	float maxSpawnInterval = 30.0f; // 最大の出現間隔（30秒）
	const float minSpawnInterval = 3.0f;  // 最短の出現間隔（10秒）
	float timeElapsed = 0.0f;  // 経過時間
	//位置
	XMFLOAT3 Left  (-2, 0, 2);
	XMFLOAT3 Right (10, 0, 2);
	XMFLOAT3 Back (4, 0, 6);
	XMFLOAT3 Front(4, 0, -4);
	
	// speedを時間経過で増加させる（最大速度を設定）
	const float maxSpeed = 0.2f;          // 最大速度の例
	const float speedIncrement = 0.005f; // 1秒あたり増加する速度の例
	int frameCount = 0;           // フレームカウント用
	const int framesPerBoost = 300; // 何フレームごとに速度を上げるか（例：300フレーム = 5秒間60fps前提）
	//敵
	const int maxEnemyCount = 4; // 最大敵数（例：10体まで出現可能）
	XMFLOAT3 EnemyLeft(-2.5, 0, 2);
	XMFLOAT3 EnemyRight(10.5, 0, 2);
	XMFLOAT3 EnemyBack(4, 0, 7);
	XMFLOAT3 EnemyFront(4, 0, -2);
	XMFLOAT3 rotLeft(0, 90, 0);
	XMFLOAT3 rotRight(0, 270, 0);
	XMFLOAT3 rotBack(0, 180, 0);
	XMFLOAT3 rotFront(0, 0, 0);
}

//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),	speed(0.03f)
{
}

//初期化
void PlayScene::Initialize()
{
	//hPlayScene = Image::Load("Model//playScene.png");
	
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
	Instantiate<Player>(this);
	Instantiate<RedWall>(this);
	Instantiate<CannonEnemy>(this);
	Instantiate<Stage>(this);
	Instantiate<Hp>(this);
	
	
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
	if (timeElapsed >= maxSpawnInterval) 
	{
		// spawnIntervalを段階的に縮めていく
		float timeFactor = (timeElapsed - maxSpawnInterval) / maxSpawnInterval;  // 30秒以降
		spawnInterval = max(minSpawnInterval, maxSpawnInterval - timeFactor * 27.0f);  // 最大5秒になるように設定
	}

	if (SpawnTimer >= spawnInterval)
	{
		//Update_CannonEnemy();
		SpawnTimer = 0.0f; // タイマーをリセット
		Update_SpawnNeedle();// 棘ボールのスポーン処理
		if (rand() % 2 == 0) // 50%の確率で味方をスポーン
		{
			Update_SpawnHeal(); // 味方のスポーン処理
		}
		//Update_SpawnBomb();
		//Update_SpawnGhost(); // ゴーストのスポーン処理
	}
	if (FindObject("CannonEnemy") == nullptr)
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMECLEAR); 
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

void PlayScene::Update_SpawnNeedle()
{
	NeedleRandom = rand() % 4; // 0～3 のランダム値
	NeedleBall* nBall = nullptr; // NeedleBallのポインタ
	switch (NeedleRandom)
	{
	case 0: // 左から
		nBall = Instantiate<NeedleBall>(this);
		nBall->SetPos(Left);
		nBall->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // 右から
		nBall = Instantiate<NeedleBall>(this);
		nBall->SetPos(Right);
		nBall->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // 奥から
		nBall = Instantiate<NeedleBall>(this);
		nBall->SetPos(Back);
		nBall->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // 手前から
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
	HealBallRandom = rand() % 4; // 0～3 のランダム値
	HealBall* hBall = nullptr; // AllyBallのポインタ
	switch (HealBallRandom)
	{
	case 0: // 左から
		hBall = Instantiate<HealBall>(this);
		hBall->SetPos(Left);
		hBall->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // 右から
		hBall = Instantiate<HealBall>(this);
		hBall->SetPos(Right);
		hBall->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // 奥から
		hBall = Instantiate<HealBall>(this);
		hBall->SetPos(Back);
		hBall->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // 手前から
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
	BombRandom = rand() % 4; // 0～3 のランダム値
	Bomb* bomb = nullptr; // Bombのポインタ
	switch (BombRandom)
	{
	case 0: // 左から
		bomb = Instantiate<Bomb>(this);
		bomb->SetPos(Left);
		bomb->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // 右から
		bomb = Instantiate<Bomb>(this);
		bomb->SetPos(Right);
		bomb->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // 奥から
		bomb = Instantiate<Bomb>(this);
		bomb->SetPos(Back);
		bomb->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // 手前から
		bomb = Instantiate<Bomb>(this);
		bomb->SetPos(Front);
		bomb->SetMove(XMFLOAT3(0, 0, speed));
		break;
	default:
		break;
	}
}

void PlayScene::Update_SpawnGhost()
{
	float GhostSpeed = 0.01f; // Ghostの移動速度
	Ghost* ghost = nullptr; // Ghostのポインタ
	GhostRandom = rand() % 4; // 0～3 のランダム値
	switch (GhostRandom)
	{
	case 0: // 左から
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Left);
		ghost->SetMove(XMFLOAT3(GhostSpeed, 0, 0));
		ghost->SetRot(XMFLOAT3(0, 90, 0)); // 左からの向き
		break;
	case 1: // 右から
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Right);
		ghost->SetMove(XMFLOAT3(-GhostSpeed, 0, 0));
		ghost->SetRot(XMFLOAT3(0, 270, 0)); // 右からの向き
		break;
	case 2: // 奥から
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Back);
		ghost->SetMove(XMFLOAT3(0, 0, -GhostSpeed));
		ghost->SetRot(XMFLOAT3(0, 180, 0)); // 奥からの向き
		break;
	case 3: // 手前から
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Front);
		ghost->SetMove(XMFLOAT3(0, 0, GhostSpeed));
		ghost->SetRot(XMFLOAT3(0, 0, 0)); // 手前からの向き
		break;
	default:
		break;
	}
}

void PlayScene::Update_CannonEnemy()
{
	CannonEnemy* cEnemy = nullptr;
	for (int i = 0; i < EnemyNum; i++)
	{
		switch (i)
		{
		case 0: // 左から
			Instantiate<CannonEnemy>(this);
			cEnemy->SetPos(EnemyLeft);
			cEnemy->SetRot(rotLeft);
			break;
		case 1: // 右から
			Instantiate<CannonEnemy>(this);
			cEnemy->SetPos(EnemyRight);
			cEnemy->SetRot(rotRight);
			break;
		case 2: // 奥から
			Instantiate<CannonEnemy>(this);
			cEnemy->SetPos(EnemyBack);
			cEnemy->SetRot(rotBack);
			break;
		case 3: // 手前から
			Instantiate<CannonEnemy>(this);
			cEnemy->SetPos(EnemyFront);
			cEnemy->SetRot(rotFront);
			break;
		default:
			break;
		}
	}
}

float PlayScene::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}