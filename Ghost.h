#pragma once
#include "Engine/GameObject.h"
class Ghost :
    public GameObject
{
	int hGhost;
	XMFLOAT3 moveDirection;
public:
	Ghost(GameObject* parent);
	~Ghost();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	void OnCollision(GameObject* pTarget) override;
	void SetPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetMove(XMFLOAT3 move) { moveDirection = move; }
	void SetRot(XMFLOAT3 rot) { transform_.rotate_ = rot; }
};

