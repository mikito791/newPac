#pragma once
#include "Engine/GameObject.h"
#include"Engine/Global.h"
#include"Engine/CsvReader.h"
#include"FlashLight.h"
#include"Shield.h"

class Player :
    public GameObject
{
	int hPlayer;
	int hConfusion;//混乱エフェクトのモデル
	Transform ConTrans;// 混乱エフェクトのTransform
	bool IsConfusion; // 混乱状態かどうか
	std::chrono::steady_clock::time_point lastUpdateTime; // 最後の更新時間
	
	bool onGround; // 地面にいるかどうか（地面ないけど）
	float HP; // 現在のHP
	float MaxHP; // 最大HP
	CsvReader csv;
	bool onReversal;
	float reversalTimer;
	int hHealSound;
	int hDamageSound;
	Shield* pShield = nullptr;
	FlashLight* pFlashLight = nullptr;
	bool prevSpaceKey;// 前回のスペースキーの状態
	Transform ShieldTrans;//シールドのTransform
	Transform FlashLightTrans;//懐中電灯のTransform
	XMFLOAT3 ShieldPos;
	XMFLOAT3 FlashLightPos;
	int ShieldRotY;
	int FlashLightRotY;
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	void OnCollision(GameObject* pTarget) override;
	void StartReversal()
	{
		onReversal = true;
		reversalTimer = 0.0f;
	}
private:
	Direction GetDirectionFromInput() const;
	int GetRotationFromDirection(Direction dir);
	XMFLOAT3 GetPositionFromDirection(Direction dir);
	void HpDown(float hp);
	float GetDeltaTime();
	void HpUp(int hp);
	void Invincible();
};

