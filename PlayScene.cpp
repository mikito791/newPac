#include "PlayScene.h"
#include"Player.h"
#include"Shield.h"
#include"NeedleBall.h"
#include"Hp.h"
#include"HealBall.h"
#include"Bomb.h"
#include"Engine/Debug.h"
#include"Engine/Image.h"
#include"Stage.h"
#include"ReversalBall.h"
#include"Ghost.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

namespace
{
	//スポーン関係
	float SpawnTimer = 0.0f;
	float spawnInterval = 30.0f; // 敵を30秒ごとに出現させる
	float maxSpawnInterval = 30.0f; // 最大の出現間隔（30秒）
	const float minSpawnInterval = 3.0f;  // 最短の出現間隔（10秒）
	float timeElapsed = 0.0f;  // 経過時間
	//位置
	XMFLOAT3 Left(-2, 0, 2);
	XMFLOAT3 Right(10, 0, 2);
	XMFLOAT3 Back(4, 0, 6);
	XMFLOAT3 Front(4, 0, -1.5);

	// speedを時間経過で増加させる（最大速度を設定）
	const float maxSpeed = 0.2f;          // 最大速度の例
	const float speedIncrement = 0.005f; // 1秒あたり増加する速度の例
	int frameCount = 0;           // フレームカウント用
	const int framesPerBoost = 300; // 何フレームごとに速度を上げるか（例：300フレーム = 5秒間60fps前提）
	//敵
	const int maxEnemyCount = 1; // 最大敵数（例：4体まで出現可能）今は3右の敵が体力無限状態になってる
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
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
	
	hPlayScene = -1; // モデルハンドルの初期化
	speed = 0.03f; // 初期の移動速度を設定
	SpawnTimer = 0.0f; // スポーンタイマーの初期化
	spawnInterval = 30.0f; // 初期のスポーン間隔を30秒に設定
	timeElapsed = 0.0f; // 経過時間の初期化
	frameCount = 0; // フレームカウントの初期化
	hBGM = -1; // BGMのハンドル初期化
}

//初期化
void PlayScene::Initialize()
{
	//hPlayScene = Image::Load("Model//playScene.png");
	csv.Load("CSV/Enemy.csv"); // CSVファイルの読み込み
	hBGM = Audio::Load("Sound/StageBGM.wav"); // BGMの読み込み
	assert(hBGM >= 0);
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
	Instantiate<Player>(this);
	Instantiate<Shield>(this);
	Instantiate<Stage>(this);
	Instantiate<Hp>(this);
	Update_CannonEnemy();
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

		SpawnTimer = 0.0f; // タイマーをリセット
		
		if (rand() % 2 == 0) // 50%の確率で味方をスポーン
		{
			Update_SpawnBomb(); //爆弾のスポーン処理
			Update_SpawnHeal(); // 味方のスポーン処理
		}
		else
		{
			Update_SpawnNeedle();// 棘ボールのスポーン処理
			Update_SpawnGhost(); // ゴーストのスポーン処理
		}
		if (rand() % 5 == 0) // 1/5の確率で反転ボールをスポーン
		{
			Update_SpawnReversalBall(); // 反転ボールのスポーン処理
		}
	}
	if (FindObject("CannonEnemy") == nullptr)
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMECLEAR);
	}
	Audio::Play(hBGM); // BGMを再生
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
	int NeedleRandom = rand() % 4; // 0～3 のランダム値
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
	int HealBallRandom = rand() % 4; // 0～3 のランダム値
	HealBall* hBall = nullptr; // HealBallのポインタ
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
	int BombRandom = rand() % 4; // 0～3 のランダム値
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

void PlayScene::Update_SpawnReversalBall()
{
	int ReversalBallRandom = rand() % 4; // 0～3 のランダム値
	ReversalBall* rBall = nullptr; // ReversalBallのポインタ
	switch (ReversalBallRandom)
	{
	case 0: // 左から
		rBall = Instantiate<ReversalBall>(this);
		rBall->SetPos(Left);
		rBall->SetMove(XMFLOAT3(speed, 0, 0));
		break;
	case 1: // 右から
		rBall = Instantiate<ReversalBall>(this);
		rBall->SetPos(Right);
		rBall->SetMove(XMFLOAT3(-speed, 0, 0));
		break;
	case 2: // 奥から
		rBall = Instantiate<ReversalBall>(this);
		rBall->SetPos(Back);
		rBall->SetMove(XMFLOAT3(0, 0, -speed));
		break;
	case 3: // 手前から
		rBall = Instantiate<ReversalBall>(this);
		rBall->SetPos(Front);
		rBall->SetMove(XMFLOAT3(0, 0, speed));
		break;
	default:
		break;
	}
}

void PlayScene::Update_SpawnGhost()
{
	float GhostSpeed = 0.02f; // Ghostの移動速度
	Ghost* ghost = nullptr; // Ghostのポインタ
	int GhostRandom = rand() % 4; // 0～3 のランダム値
	switch (GhostRandom)
	{
	case 0: // 左から
		
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Left);
		ghost->SetMove(XMFLOAT3(GhostSpeed, 0, 0));
		ghost->SetRot(rotLeft); // 左からの向き
		break;
	case 1: // 右から
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Right);
		ghost->SetMove(XMFLOAT3(-GhostSpeed, 0, 0));
		ghost->SetRot(rotRight); // 右からの向き
		break;
	case 2: // 奥から
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Back);
		ghost->SetMove(XMFLOAT3(0, 0, -GhostSpeed));
		ghost->SetRot(rotBack); // 奥からの向き
		break;
	case 3: // 手前から
		ghost = Instantiate<Ghost>(this);
		ghost->SetPos(Front);
		ghost->SetMove(XMFLOAT3(0, 0, GhostSpeed));
		ghost->SetRot(rotFront); // 手前からの向き
		break;
	default:
		break;
	}
}

void PlayScene::Update_CannonEnemy()
{
	XMFLOAT3 positions[] = { EnemyLeft, EnemyBack,EnemyFront,EnemyRight};
	XMFLOAT3 rotations[] = { rotLeft, rotBack,rotFront,rotRight, };

	enemies.clear(); // 既存の敵をクリア

	for (int i = 0; i < maxEnemyCount; ++i)
	{
		CannonEnemy* enemy = Instantiate<CannonEnemy>(this);
		enemy->SetPos(positions[i]);
		enemy->SetRot(rotations[i]);
		enemies.push_back(enemy);
	}
}

float PlayScene::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}