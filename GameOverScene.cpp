#include "GameOverScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"


GameOverScene::GameOverScene(GameObject* parent)
	: GameObject(parent, "GameOver")
{
	hGameOver = -1;
	hBackground = -1;
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	hGameOver = Image::Load("Model//GameOver1.png");
	assert(hGameOver >= 0);

	hBackground = Image::Load("Model//Background.png");
	assert(hBackground >= 0);
}

void GameOverScene::Update()
{
	if (Input::IsKeyDown(DIK_LSHIFT))
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	Image::Draw(hBackground);
	Image::Draw(hGameOver);
}

void GameOverScene::Release()
{
}
