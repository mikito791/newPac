#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"RedEnemy.h"
#include"Spawner.h"

//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene"),Left(-2, 0, 2),Right(10, 0, 2),Back(4, 0, 6),Front(4, 0, -4),
	speed(0.05f)
{
}

//初期化
void PlayScene::Initialize()
{
	Instantiate<Player>(this);
	Instantiate<RedWall>(this);
	//Instantiate<RedEnemy>(this);
}

//更新
void PlayScene::Update()
{
	counter -= 1;
	if (counter <= 0)
	{
		counter = 50;
		EnemyRandom = rand() % 4; // 0～3 のランダム値
		RedEnemy* Renemy = nullptr;
		switch (EnemyRandom)
		{
		case 0: // 左から
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Left);
			Renemy->SetMove(XMFLOAT3(speed, 0, 0));
			break;
		case 1: // 右から
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Right);
			Renemy->SetMove(XMFLOAT3(-speed, 0, 0));
			break;
		case 2: // 奥から
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Back);
			Renemy->SetMove(XMFLOAT3(0, 0, -speed));
			break;
		case 3: // 手前から
			Renemy = Instantiate<RedEnemy>(this);
			Renemy->SetPos(Front);
			Renemy->SetMove(XMFLOAT3(0, 0, speed));
			break;
		default:
			break;
		}
	}
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{
}
