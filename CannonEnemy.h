#pragma once
#include "Engine/GameObject.h"
class CannonEnemy :
    public GameObject
{
	int hCannonEnemy; // モデルハンドル
	int EnemyHP; // 敵のHP
public:
	CannonEnemy(GameObject* parent);
	~CannonEnemy();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	void SetPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetRot(XMFLOAT3 rot) { transform_.rotate_ = rot; }
	void HpDown(int hp);
};

