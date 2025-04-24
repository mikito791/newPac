#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"RedEnemy.h"
#include"Spawner.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene")
{
}

//������
void PlayScene::Initialize()
{
	Instantiate<Player>(this);
	Instantiate<RedWall>(this);
	//Instantiate<RedEnemy>(this);
}

//�X�V
void PlayScene::Update()
{
	counter -= 1;
	if (counter <= 0)
	{
		counter = 50;
		EnemyRandom = rand() % 4; // 0�`3 �̃����_���l
		RedEnemy* Renemy = nullptr;
		switch (EnemyRandom)
		{
		case 0: // ������
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(XMFLOAT3(-2, 0, 2));
			Renemy->SetMove(XMFLOAT3(0.05f, 0, 0));
			break;
		case 1: // �E����
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(XMFLOAT3(10, 0, 2));
			Renemy->SetMove(XMFLOAT3(-0.05f, 0, 0));
			break;
		case 2: // ������
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(XMFLOAT3(4, 0, 6));
			Renemy->SetMove(XMFLOAT3(0, 0, -0.05f));
			break;
		case 3: // ��O����
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(XMFLOAT3(4, 0, -4));
			Renemy->SetMove(XMFLOAT3(0, 0, 0.05f));
			break;
		default:
			break;
		}
	}
}

//�`��
void PlayScene::Draw()
{
}

//�J��
void PlayScene::Release()
{
}
