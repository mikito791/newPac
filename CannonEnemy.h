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
	void SetPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetRot(XMFLOAT3 rot) { transform_.rotate_ = rot; }
private:
	float GetDeltaTime();
	void HpDown(int hp);
};

