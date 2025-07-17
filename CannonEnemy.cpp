#include "CannonEnemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Debug.h"
#include "Engine/SceneManager.h"

namespace
{
	XMFLOAT3 Left(-2, 0, 2);
	XMFLOAT3 Right(10, 0, 2);
	XMFLOAT3 Back(4, 0, 6);
	XMFLOAT3 Front(4, 0, -1.5);
}

CannonEnemy::CannonEnemy(GameObject* parent)
	: GameObject(parent, "CannonEnemy")
{
	hCannonEnemy = -1; // モデルハンドルの初期化
	EnemyHP = 3; // 敵のHPの初期値設定
	attackInterval = 4.0f + static_cast<float>(rand() % 2000) / 1000.0f; // 4〜6秒
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
	float deltaTime = GetDeltaTime(); // PlaySceneとか共通で作ったdeltaTime
	attackTimer += deltaTime;

	if (attackTimer >= attackInterval)
	{
		attackTimer = 0.0f;
		attackInterval = 4.0f + static_cast<float>(rand() % 2000) / 1000.0f;

		//int attackType = rand() % 2; // 0: Bomb, 1: NeedleBall
		//if (attackType == 0)
		FireBomb();
			
	}
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
			EnemyHP = 0; // HPが0未満にならないように制限
			this->KillMe(); // HPが0以下になったら自分を削除
		}
	}
}

void CannonEnemy::HpDown(int hp)
{
	EnemyHP -= hp; // HPを減らす
}

void CannonEnemy::FireBomb()
{
	Bomb* bomb = Instantiate<Bomb>(this);
	

	float speed = 0.05f;
	float yRot = this->GetRot().y;
	if (yRot == 90.0f)//左
	{
		bomb->SetPos(Left);
		bomb->SetMove(XMFLOAT3(0, 0, speed));
	}
	else if (yRot == 270.0f)//右
	{
		bomb->SetPos(Right);
		bomb->SetMove(XMFLOAT3(0, 0, -speed));
	}
	else if (yRot == 0.0f)//手前
	{
		bomb->SetPos(Front);
		bomb->SetMove(XMFLOAT3(speed, 0, 0));
	}
	else if (yRot == 180.0f)//奥
	{
		bomb->SetPos(Back);
		bomb->SetMove(XMFLOAT3(-speed, 0, 0));
	}
}

void CannonEnemy::FireNeedle()
{
}

void CannonEnemy::FireReversalBall()
{
}

void CannonEnemy::FireHealBall()
{
}

float CannonEnemy::GetDeltaTime()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	return deltaTime.count();
}
