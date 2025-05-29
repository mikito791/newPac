#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent):
	GameObject(parent,"Title")
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	hTitle = Image::Load("Model//Title.png");
}

void TitleScene::Update()
{
	if (Input::IsKey(DIK_SPACE))
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_PLAY);
	}
}

void TitleScene::Draw()
{
	Image::Draw(hTitle);
}

void TitleScene::Release()
{
}
