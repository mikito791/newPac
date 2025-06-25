#pragma once
#include "Engine/GameObject.h"
#include <chrono>

//�v���C�V�[�����Ǘ�����N���X
class PlayScene : public GameObject
{
	int NeedleRandom;
	int AllyRandom;
	int BombRandom;
	int hPlayScene;
	float speed;
	float spawnInterval; // �X�|�[���Ԋu�i�b�j
	float timeSinceLastSpawn; // �Ō�̃X�|�[������̌o�ߎ���
	std::chrono::steady_clock::time_point lastUpdateTime; // �Ō�̍X�V����

	float GetDeltaTime();
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
private:
	void Update_SpawnNeedle();
	void Update_SpawnAlly();
	void Update_SpawnBomb();
};