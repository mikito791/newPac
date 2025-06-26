#pragma once
#include "Engine/GameObject.h"
class CannonEnemy :
    public GameObject
{
	int hCannonEnemy; // ƒ‚ƒfƒ‹ƒnƒ“ƒhƒ‹
	int EnemyHP; // “G‚ÌHP]
public:
	CannonEnemy(GameObject* parent);
	~CannonEnemy();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	void SetPosition(XMFLOAT3 pos) { transform_.position_ = pos; }
	void HpDown(int hp);
};

