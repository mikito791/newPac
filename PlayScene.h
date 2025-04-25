#pragma once
#include "Engine/GameObject.h"
#include <chrono>

//�v���C�V�[�����Ǘ�����N���X
class PlayScene : public GameObject
{
	int EnemyRandom;
	int counter;
	XMFLOAT3 Left;
	XMFLOAT3 Right;
	XMFLOAT3 Back;
	XMFLOAT3 Front;
	float speed;
	float spawnInterval; // �X�|�[���Ԋu�i�b�j
	float timeSinceLastSpawn; // �Ō�̃X�|�[������̌o�ߎ���
	std::chrono::steady_clock::time_point lastUpdateTime; // �Ō�̍X�V����
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	PlayScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};