#pragma once
#include "Engine/GameObject.h"
#include "Engine/Global.h"
class ReversalBall :
    public GameObject
{
	int hModel;
	int randomNum; // ランダムな数値
	XMFLOAT3 moveDirection; // 移動方向
	int speed; // 移動速度
public:
	ReversalBall(GameObject* parent);
	~ReversalBall();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
	Direction GetReveralDirectionFromInput();
	int GetRotationFromReveralDirection(Direction dir);
	XMFLOAT3 GetPositionFromReveralDirection(Direction dir);
	void SetPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetMove(XMFLOAT3 move) { moveDirection = move; }
};

