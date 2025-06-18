#pragma once
#include "Engine/GameObject.h"
#include"Engine/Global.h"
class Player :
    public GameObject
{
	int hModel;
	XMVECTOR pos;
	bool isInvincible; // ���G��Ԃ��ǂ���
	float invincibilityTimer; // ���G���Ԃ̃J�E���g
	std::chrono::steady_clock::time_point lastUpdateTime; // �Ō�̍X�V����
	bool prevSpaceKey;// �O��̃X�y�[�X�L�[�̏��
	float jumpPower; // �W�����v�̗�
	bool onGround; // �n�ʂɂ��邩�ǂ����i�n�ʂȂ����ǁj
	int HP;
	int Point;
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	float CalculateDistance(const XMFLOAT3& PlayPos, const XMFLOAT3& Pos);
	void OnCollision(GameObject* pTarget) override;
private:
	Direction GetDirectionFromInput();
	int GetRotationFromDirection(Direction dir);
	void HpDown(int hp);
	float GetDeltaTime();
	void PointUp(int pt);
	void Jump();
	void Invincible();
};

