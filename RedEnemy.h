#pragma once
#include "Engine/GameObject.h"
class RedEnemy :
    public GameObject
{
    int hRedEnemy;
	XMFLOAT3 moveDirection;
	int num;
	Transform transform_; // Transformメンバ変数
public:
	
	RedEnemy(GameObject* parent);
	~RedEnemy();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void OnCollision(GameObject* pTarget) override;
	void SetPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetMove(XMFLOAT3 move) { moveDirection = move; }
	XMFLOAT3 GetPos() { return transform_.position_; }
	float CalculateDistancePlayer(const XMFLOAT3& EnemyPos, const XMFLOAT3& Playerpos);
	float CalculateDistanceWall(const XMFLOAT3& EnemyPos, const XMFLOAT3& Wallpos);
};

