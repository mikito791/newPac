#include "TestScene.h"
#include"Player.h"
#include"RedWall.h"
#include"RedEnemy.h"

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������
void TestScene::Initialize()
{
	Instantiate<Player>(this);
	Instantiate<RedWall>(this);
	Instantiate<RedEnemy>(this);
}

//�X�V
void TestScene::Update()
{
}

//�`��
void TestScene::Draw()
{
}

//�J��
void TestScene::Release()
{
}
