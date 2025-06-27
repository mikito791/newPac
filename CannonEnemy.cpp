#include "CannonEnemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Debug.h"
#include "Engine/SceneManager.h"
#include"NeedleBall.h"
#include"Bomb.h"

CannonEnemy::CannonEnemy(GameObject* parent)
	: GameObject(parent, "CannonEnemy")
{
	hCannonEnemy = -1; // モデルハンドルの初期化
	EnemyHP = 3; // 敵のHPの初期値設定
	EnemyNum = 4;
}

CannonEnemy::~CannonEnemy()
{
}

void CannonEnemy::Initialize()
{
	hCannonEnemy = Model::Load("Model/CannonEnemy.fbx");
	assert(hCannonEnemy >= 0);
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.01f);
	AddCollider(collider);
}

void CannonEnemy::Update()
{
	//transform_.position_ = XMFLOAT3(-2.5, 0, 2);
	//transform_.rotate_.y = 90; // 左からの向き	
}

void CannonEnemy::Draw()
{
	Model::SetTransform(hCannonEnemy, transform_);
	Model::Draw(hCannonEnemy);	
}

void CannonEnemy::Release()
{
}

void CannonEnemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "NeedleBall")
	{
		HpDown(1); // Needleに当たったらHPを1減らす
		pTarget->KillMe(); // Needleに当たったら自分を削除
		if (EnemyHP <= 0)
		{
			this->KillMe(); // HPが0以下になったら自分を削除
		}
	}
}

void CannonEnemy::HpDown(int hp)
{
	EnemyHP -= hp; // HPを減らす
}
