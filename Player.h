#pragma once
#include "Engine/GameObject.h"
#include"Engine/Global.h"
#include"Engine/CsvReader.h"
class Player :
    public GameObject
{
	int hModel;
	bool isInvincible; // 無敵状態かどうか
	float invincibilityTimer; // 無敵時間のカウント
	std::chrono::steady_clock::time_point lastUpdateTime; // 最後の更新時間
	bool prevSpaceKey;// 前回のスペースキーの状態
	float jumpPower; // ジャンプの力
	bool onGround; // 地面にいるかどうか（地面ないけど）
	float HP; // 現在のHP
	float MaxHP; // 最大HP
	CsvReader csv;
	int hDmageSound; // ダメージ音のハンドル
	int hHealSound; // 回復音のハンドル
	int hBombSound; // 爆弾音のハンドル
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	float CalculateDistance(const XMFLOAT3& PlayPos, const XMFLOAT3& Pos);
	void OnCollision(GameObject* pTarget) override;
private:
	Direction GetDirectionFromInput();
	int GetRotationFromDirection(Direction dir);
	void HpDown(float hp);
	float GetDeltaTime();
	void HpUp(int pt);
	void Jump();
	void Invincible();
};

