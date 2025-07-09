#pragma once
#include "Engine/GameObject.h"
#include"Engine/Global.h"
#include"Engine/CsvReader.h"
class Player :
    public GameObject
{
	int hPlayer;
	bool isInvincible; // ���G��Ԃ��ǂ���
	float invincibilityTimer; // ���G���Ԃ̃J�E���g
	std::chrono::steady_clock::time_point lastUpdateTime; // �Ō�̍X�V����
	bool prevSpaceKey;// �O��̃X�y�[�X�L�[�̏��
	float jumpPower; // �W�����v�̗�
	bool onGround; // �n�ʂɂ��邩�ǂ����i�n�ʂȂ����ǁj
	float HP; // ���݂�HP
	float MaxHP; // �ő�HP
	CsvReader csv;
	bool onReversal;
	float reversalTimer;
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
	void StartReversal()
	{
		onReversal = true;
		reversalTimer = 0.0f;
	}
	bool IsOnReversal()const { return onReversal; }
private:
	Direction GetDirectionFromInput() const;
	int GetRotationFromDirection(Direction dir);
	void HpDown(float hp);
	float GetDeltaTime();
	void HpUp(int pt);
	void Jump();
	void Invincible();
	
};

