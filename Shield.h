#pragma once
#include "Engine/GameObject.h"
#include "Engine/Global.h"
#include"Engine/CsvReader.h"
#include"Player.h"
class Shield :
    public GameObject
{
	int hShield;
	CsvReader csv;
	Player* pPlayer; // �v���C���[�I�u�W�F�N�g�ւ̃|�C���^
public:
	Shield(GameObject* parent);
	~Shield();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
	void SetPosition(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetDirection(Direction dir) { transform_.rotate_.y = GetRotationFromDirection(dir); }
	XMFLOAT3 GetPos() const { return transform_.position_; }
	void SetPlayer(Player* player) { pPlayer = player; } // �v���C���[�I�u�W�F�N�g��ݒ�
	void SetTransform(const XMFLOAT3& pos, int rotY)
	{
		transform_.position_ = pos;
		transform_.rotate_.y = rotY;
	}
private:
	Direction GetDirectionFromInput();
	int GetRotationFromDirection(Direction dir);
	XMFLOAT3 GetPositionFromDirection(Direction dir);
};

