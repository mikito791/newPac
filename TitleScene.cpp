#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"


TitleScene::TitleScene(GameObject* parent):
	GameObject(parent,"Title")
{
	hTitle = -1; // モデルハンドルの初期化
	alpha = 255; // 初期のアルファ値を設定
	hBackground = -1; // 背景画像のハンドル初期化
	isFadeOut = false; // フェードアウト状態を初期化
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	hTitle = Image::Load("Model//Title2.png");
	assert(hTitle >= 0); // 画像の読み込みが成功したか確認
	hBackground = Image::Load("Model//Background.png");
	assert(hBackground >= 0); // 背景画像の読み込みが成功したか確認
}

void TitleScene::Update()
{
	if (!isFadeOut&&Input::IsKey(DIK_SPACE))
	{
		isFadeOut = true; // スペースキーが押されたらフェードアウト開始
	}
	if (isFadeOut)
	{
		alpha -= 2; // アルファ値を減少させる（フェードアウト効果）
		if (alpha <= 0)
		{
			alpha = 0; // アルファ値が0未満にならないように制限
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_PLAY);
			isFadeOut = false;
		}
	}
}

void TitleScene::Draw()
{
	Image::Draw(hBackground); // 背景画像を描画
	Image::SetAlpha(hTitle, alpha); // アルファ値を設定
	Image::Draw(hTitle);
}

void TitleScene::Release()
{
}
