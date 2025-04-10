#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
	int hModel;
	XMVECTOR front;
	XMVECTOR pos;
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void OnCollision(GameObject* pTarget) override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	
};

