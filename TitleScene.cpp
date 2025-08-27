#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"


TitleScene::TitleScene(GameObject* parent):
	GameObject(parent,"Title")
{
	hTitle = -1; // ���f���n���h���̏�����
	alpha = 255; // �����̃A���t�@�l��ݒ�
	hBackground = -1; // �w�i�摜�̃n���h��������
	isFadeOut = false; // �t�F�[�h�A�E�g��Ԃ�������
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	hTitle = Image::Load("Model//Title2.png");
	assert(hTitle >= 0); // �摜�̓ǂݍ��݂������������m�F
	hBackground = Image::Load("Model//Background.png");
	assert(hBackground >= 0); // �w�i�摜�̓ǂݍ��݂������������m�F
}

void TitleScene::Update()
{
	if (!isFadeOut&&Input::IsKey(DIK_SPACE))
	{
		isFadeOut = true; // �X�y�[�X�L�[�������ꂽ��t�F�[�h�A�E�g�J�n
	}
	if (isFadeOut)
	{
		alpha -= 2; // �A���t�@�l������������i�t�F�[�h�A�E�g���ʁj
		if (alpha <= 0)
		{
			alpha = 0; // �A���t�@�l��0�����ɂȂ�Ȃ��悤�ɐ���
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_PLAY);
			isFadeOut = false;
		}
	}
}

void TitleScene::Draw()
{
	Image::Draw(hBackground); // �w�i�摜��`��
	Image::SetAlpha(hTitle, alpha); // �A���t�@�l��ݒ�
	Image::Draw(hTitle);
}

void TitleScene::Release()
{
}
