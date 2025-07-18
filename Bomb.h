#pragma once
#include "Engine/GameObject.h"

class Bomb :
    public GameObject
{
    int hBomb;
	XMFLOAT3 moveDirection;
	int hBombSound;
public:
    Bomb(GameObject* parent);
    ~Bomb();
	
	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void SetPos(const XMFLOAT3& pos) { transform_.position_ = pos; }
	void SetMove(const XMFLOAT3& move) { moveDirection = move; }
	XMFLOAT3 GetPos() { return transform_.position_; }
	void OnCollision(GameObject* pTarget) override;
private:
	void BombEffect();
};


