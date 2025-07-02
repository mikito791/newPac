#pragma once
#include "Engine/GameObject.h"
#include <chrono>
#include"Engine/CsvReader.h"

//�v���C�V�[�����Ǘ�����N���X
class PlayScene : public GameObject
{
	
	int hPlayScene;
	float speed;
	float spawnInterval; // �X�|�[���Ԋu�i�b�j
	std::chrono::steady_clock::time_point lastUpdateTime; // �Ō�̍X�V����
	CsvReader csv; 
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
	void Update_SpawnHeal();
	void Update_SpawnBomb();
	void Update_SpawnReversalBall();
	void Update_SpawnGhost();
	void Update_CannonEnemy();
	float GetDeltaTime();
};