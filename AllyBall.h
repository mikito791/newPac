#pragma once
#include "Engine/GameObject.h"
class AllyBall :
    public GameObject
{
	int hAllyBall; // アリーのボールのモデルハンドル
	XMFLOAT3 moveDirection; // 移動方向
	float speed; // 移動速度
	float distance; // 衝突判定の距離
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	AllyBall(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

};

