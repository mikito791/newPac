#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"RedEnemy.h"
#include"Spawner.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),Left(-2, 0, 2),Right(10, 0, 2),Back(4, 0, 6),Front(4, 0, -4),
	speed(0.05f)
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
			Renemy->SetPos(Left);
			Renemy->SetMove(XMFLOAT3(speed, 0, 0));
			break;
		case 1: // �E����
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Right);
			Renemy->SetMove(XMFLOAT3(-speed, 0, 0));
			break;
		case 2: // ������
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Back);
			Renemy->SetMove(XMFLOAT3(0, 0, -speed));
			break;
		case 3: // ��O����
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Front);
			Renemy->SetMove(XMFLOAT3(0, 0, speed));
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
