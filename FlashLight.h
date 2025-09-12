#pragma once
#include "Engine/GameObject.h"
#include "Engine/Global.h"
#include"Engine/CsvReader.h"
#include <chrono>

class FlashLight :
    public GameObject
{
    int hFlashLight;
	CsvReader csv;
	bool onReversal;
	float reversalTimer;
	std::chrono::steady_clock::time_point lastUpdateTime; // ç≈å„ÇÃçXêVéûä‘
public:
	FlashLight(GameObject* parent);
	~FlashLight();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void StartReversalLight()
	{
		onReversal = true;
		reversalTimer = 0.0f;
	}
	void SetTransform(const XMFLOAT3& pos, int rotY)
	{
		transform_.position_ = pos;
		transform_.rotate_.y = rotY;
	}
private:
	float GetDeltaTime();
	Direction GetDirectionFromInput() const;
	int GetRotationFromDirection(Direction dir);
	XMFLOAT3 GetPositionFromDirection(Direction dir);
};

