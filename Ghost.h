#pragma once
#include "Engine/GameObject.h"
class Ghost :
    public GameObject
{
	int hGhost;
public:
	Ghost(GameObject* parent);
	~Ghost();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	void OnCollision(GameObject* pTarget) override;
};

