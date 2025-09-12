#pragma once
#include "Engine/GameObject.h"
#include"Engine/Global.h"
#include"Engine/CsvReader.h"
#include"FlashLight.h"
#include"Shield.h"

class Player :
    public GameObject
{
	int hPlayer;
	int hConfusion;//�����G�t�F�N�g�̃��f��
	Transform ConTrans;// �����G�t�F�N�g��Transform
	bool IsConfusion; // ������Ԃ��ǂ���
	std::chrono::steady_clock::time_point lastUpdateTime; // �Ō�̍X�V����
	
	bool onGround; // �n�ʂɂ��邩�ǂ����i�n�ʂȂ����ǁj
	float HP; // ���݂�HP
	float MaxHP; // �ő�HP
	CsvReader csv;
	bool onReversal;
	float reversalTimer;
	int hHealSound;
	int hDamageSound;
	Shield* pShield = nullptr;
	FlashLight* pFlashLight = nullptr;
	bool prevSpaceKey;// �O��̃X�y�[�X�L�[�̏��
	Transform ShieldTrans;//�V�[���h��Transform
	Transform FlashLightTrans;//�����d����Transform
	XMFLOAT3 ShieldPos;
	XMFLOAT3 FlashLightPos;
	int ShieldRotY;
	int FlashLightRotY;
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	XMFLOAT3 GetPos() const { return transform_.position_; }
	void OnCollision(GameObject* pTarget) override;
	void StartReversal()
	{
		onReversal = true;
		reversalTimer = 0.0f;
	}
private:
	Direction GetDirectionFromInput() const;
	int GetRotationFromDirection(Direction dir);
	XMFLOAT3 GetPositionFromDirection(Direction dir);
	void HpDown(float hp);
	float GetDeltaTime();
	void HpUp(int hp);
	void Invincible();
};

