#pragma once
#include "Engine/GameObject.h"
#include"NeedleBall.h"
#include"Bomb.h"
#include"ReversalBall.h"
#include "HealBall.h"
#include <chrono>

class CannonEnemy :
    public GameObject
{
	int hCannonEnemy; // ���f���n���h��
	int EnemyHP; // �G��HP
	std::chrono::steady_clock::time_point lastUpdateTime; // �Ō�̍X�V����
	float attackTimer; // �U���^�C�}�[
	float attackInterval; // �U���Ԋu�i�b�j
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
private:
	float AttackTimer = 0.0f; // �U���^�C�}�[
	float AttackInterval = 5.0f; // �U���Ԋu�i�b�j

	void FireBomb();
	void FireNeedle();
	void FireReversalBall();
	void FireHealBall();
	float GetDeltaTime();
	XMFLOAT3 GetRot() { return transform_.rotate_; }
};

