#pragma once
#include "Engine/GameObject.h"
#include"Engine/Global.h"
#include"Engine/CsvReader.h"
class Player :
    public GameObject
{
	int hPlayer;
	int hConfusion;//�����G�t�F�N�g�̃��f��
	Transform ConTrans;// �����G�t�F�N�g��Transform
	bool IsConfusion; // ������Ԃ��ǂ���
	std::chrono::steady_clock::time_point lastUpdateTime; // �Ō�̍X�V����
	bool prevSpaceKey;// �O��̃X�y�[�X�L�[�̏��
	float jumpPower; // �W�����v�̗�
	bool onGround; // �n�ʂɂ��邩�ǂ����i�n�ʂȂ����ǁj
	float HP; // ���݂�HP
	float MaxHP; // �ő�HP
	CsvReader csv;
	bool onReversal;
	float reversalTimer;
	int hHealSound;
	int hDamageSound;
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
private:
	Direction GetDirectionFromInput() const;
	int GetRotationFromDirection(Direction dir);
	void HpDown(float hp);
	float GetDeltaTime();
	void HpUp(int hp);
	void Jump();
	void Invincible();
};

