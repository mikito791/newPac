#pragma once
#include "Engine/GameObject.h"
#include"NeedleBall.h"
#include"Bomb.h"
#include"ReversalBall.h"
#include "HealBall.h"
#include <chrono>

class CannonEnemy :
    public GameObject
{
	int hCannonEnemy; // モデルハンドル
	int EnemyHP; // 敵のHP
	std::chrono::steady_clock::time_point lastUpdateTime; // 最後の更新時間
	float attackTimer; // 攻撃タイマー
	float attackInterval; // 攻撃間隔（秒）
public:
	CannonEnemy(GameObject* parent);
	~CannonEnemy();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	void SetPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetRot(XMFLOAT3 rot) { transform_.rotate_ = rot; }
	void HpDown(int hp);
private:
	float AttackTimer = 0.0f; // 攻撃タイマー
	float AttackInterval = 5.0f; // 攻撃間隔（秒）

	void FireBomb();
	void FireNeedle();
	void FireReversalBall();
	void FireHealBall();
	float GetDeltaTime();
	XMFLOAT3 GetRot() { return transform_.rotate_; }
};

