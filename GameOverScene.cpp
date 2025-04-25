#include "GameOverScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"


GameOverScene::GameOverScene(GameObject* parent)
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	hGameOver = Image::Load("Model//GameOver.png");
}

void GameOverScene::Update()
{
	if (Input::IsKey(DIK_SPACE))
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	Image::Draw(hGameOver);
}

void GameOverScene::Release()
{
}
