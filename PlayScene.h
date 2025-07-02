#pragma once
#include "Engine/GameObject.h"
#include <chrono>
#include"Engine/CsvReader.h"

//プレイシーンを管理するクラス
class PlayScene : public GameObject
{
	
	int hPlayScene;
	float speed;
	float spawnInterval; // スポーン間隔（秒）
	std::chrono::steady_clock::time_point lastUpdateTime; // 最後の更新時間
	CsvReader csv; 
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
private:
	void Update_SpawnNeedle();
	void Update_SpawnHeal();
	void Update_SpawnBomb();
	void Update_SpawnReversalBall();
	void Update_SpawnGhost();
	void Update_CannonEnemy();
	float GetDeltaTime();
};