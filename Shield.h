#pragma once
#include "Engine/GameObject.h"
#include "Engine/Global.h"
#include"Engine/CsvReader.h"
#include <chrono>

class Shield :
    public GameObject
{
	int hShield;
	CsvReader csv;
	bool onReversal;
	float reversalTimer;
	std::chrono::steady_clock::time_point lastUpdateTime; // ç≈å„ÇÃçXêVéûä‘
public:
	Shield(GameObject* parent);
	~Shield();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void SetPosition(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetDirection(Direction dir) { transform_.rotate_.y = GetRotationFromDirection(dir); }
	XMFLOAT3 GetPos() const { return transform_.position_; }
	void SetTransform(const XMFLOAT3& pos, int rotY)
	{
		transform_.position_ = pos;
		transform_.rotate_.y = rotY;
	}
	void StartReversal()
	{
		onReversal = true;
		reversalTimer = 0.0f;
	}
private:
	Direction GetDirectionFromInput() const;
	int GetRotationFromDirection(Direction dir);
	XMFLOAT3 GetPositionFromDirection(Direction dir);
	float GetDeltaTime();
};

