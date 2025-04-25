#pragma once
#include "Engine/GameObject.h"

//テストシーンを管理するクラス
class PlayScene : public GameObject
{
	int EnemyRandom;
	int counter;
	XMFLOAT3 Left;
	XMFLOAT3 Right;
	XMFLOAT3 Back;
	XMFLOAT3 Front;
	float speed;
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
};