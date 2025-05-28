#pragma once
#include "Engine/GameObject.h"
#include"Engine/Global.h"
class Player :
    public GameObject
{
	int hModel;
	XMVECTOR pos;
	bool isInvincible; // 無敵状態かどうか
	float invincibilityTimer; // 無敵時間のカウント
	std::chrono::steady_clock::time_point lastUpdateTime; // 最後の更新時間
	bool prevSpaceKey;
	float jumpSpeed;
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	float CalculateDistanceEnemy(const XMFLOAT3& PlayPos, const XMFLOAT3& EnemyPos);
	void OnCollision(GameObject* pTarget) override;
private:
	Direction GetDirectionFromInput();
	int GetRotationFromDirection(Direction dir);
	void HpDown(int hp);
	float GetDeltaTime();
	void PointUp(int pt);
};

