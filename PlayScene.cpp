#include "PlayScene.h"
#include"Player.h"
#include"RedWall.h"
#include"RedEnemy.h"

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
	Instantiate<RedEnemy>(this);
}

//�X�V
void PlayScene::Update()
{
}

//�`��
void PlayScene::Draw()
{
}

//�J��
void PlayScene::Release()
{
}
