#pragma once
#include "Engine/GameObject.h"
class AllyBall :
    public GameObject
{
	int hAllyBall; // �A���[�̃{�[���̃��f���n���h��
	XMFLOAT3 moveDirection; // �ړ�����
	float speed; // �ړ����x
	float distance; // �Փ˔���̋���
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	AllyBall(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

};

