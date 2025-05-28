#include "GameClearScene.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"

GameClearScene::GameClearScene(GameObject* parent)
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
{
	hGameClear = Image::Load("Model//GameClear.png");
	assert(hGameClear >= 0);
}

void GameClearScene::Update()
{
	if (Input::IsKey(DIK_LSHIFT))
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
	}
}

void GameClearScene::Draw()
{
	Image::Draw(hGameClear);
}

void GameClearScene::Release()
{
}
