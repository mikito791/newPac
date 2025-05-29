#pragma once
#include "Engine/GameObject.h"
class AllyBall :
    public GameObject
{
	int hAllyBall; // アリーのボールのモデルハンドル
	XMFLOAT3 moveDirection; // 移動方向
	float speed; // 移動速度
	float distance; // 衝突判定の距離
	int num; // ランダム値
public:
	
	AllyBall(GameObject* parent);
	~AllyBall();
	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
	void SetPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetMove(XMFLOAT3 move) { moveDirection = move; }
	void OnCollision(GameObject* pTarget) override;
};

