#include "CannonEnemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Debug.h"
#include "Engine/SceneManager.h"

enum EnemyID
{
	Left,
	Right,
	Back,
	Front,
};
CannonEnemy::CannonEnemy(GameObject* parent)
{
	hCannonEnemy = -1; // モデルハンドルの初期化
	EnemyHP = 3; // 敵のHPの初期値設定
}

CannonEnemy::~CannonEnemy()
{
}

void CannonEnemy::Initialize()
{
	hCannonEnemy = Model::Load("Model/CannonEnemy.fbx");
	assert(hCannonEnemy >= 0);
	
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collider);
}

void CannonEnemy::Update()
{
}

void CannonEnemy::Draw()
{
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0: // 左から
			transform_.position_ = XMFLOAT3(-2.5, 0, 2);
			transform_.rotate_.y = 90; // 左からの向き
			Model::SetTransform(hCannonEnemy, transform_);
			Model::Draw(hCannonEnemy);
			break;
		case 1: // 右から
			transform_.position_ = XMFLOAT3(10.5, 0, 2);
			transform_.rotate_.y = 270; // 右からの向き
			Model::SetTransform(hCannonEnemy, transform_);
			Model::Draw(hCannonEnemy);
			break;
		case 2: // 奥から
			transform_.position_ = XMFLOAT3(4, 0, 7);
			transform_.rotate_.y = 180; // 奥からの向き
			Model::SetTransform(hCannonEnemy, transform_);
			Model::Draw(hCannonEnemy);
			break;
		case 3: // 手前から
			transform_.position_ = XMFLOAT3(4, 0, -1.5);
			transform_.rotate_.y = 0; // 手前からの向き
			Model::SetTransform(hCannonEnemy, transform_);
			Model::Draw(hCannonEnemy);
			break;
		default:
			break;
		}
	}
	
}

void CannonEnemy::Release()
{
}

void CannonEnemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Needle")
	{
		HpDown(1); // Needleに当たったらHPを1減らす
		pTarget->KillMe(); // Needleに当たったら自分を削除
		if (EnemyHP <= 0)
		{
			this->KillMe(); // HPが0以下になったら自分を削除
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_GAMECLEAR); // ゲームクリア画面に遷移
		}
	}
}

void CannonEnemy::HpDown(int hp)
{
	EnemyHP -= hp; // HPを減らす
}
