#pragma once
#include "Engine/GameObject.h"
class AllyBall :
    public GameObject
{
	int hAllyBall; // �A���[�̃{�[���̃��f���n���h��
	XMFLOAT3 moveDirection; // �ړ�����
	float speed; // �ړ����x
	float distance; // �Փ˔���̋���
	int num; // �����_���l
public:
	
	AllyBall(GameObject* parent);
	~AllyBall();
	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
	void SetPos(XMFLOAT3 pos) { transform_.position_ = pos; }
	void SetMove(XMFLOAT3 move) { moveDirection = move; }
	void OnCollision(GameObject* pTarget) override;
};

