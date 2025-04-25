#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
	int hModel;
	XMVECTOR pos;
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	float CalculateDistanceEnemy(const XMFLOAT3& PlayPos, const XMFLOAT3& EnemyPos);
	//void OnCollision(GameObject* pTarget) override;
};

