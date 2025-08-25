#include "NeedleBall.h"
#include"Engine/Model.h"
#include"Player.h"

#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Debug.h"
#include"Engine/SceneManager.h"
#include"Engine/SphereCollider.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"

NeedleBall::NeedleBall(GameObject* parent)
	: GameObject(parent, "NeedleBall")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
	speed = 0.05f; // 移動速度
	distance = 0.001f; // 衝突判定の距離
	hDamageSound = -1; // ダメージ音のハンドル
}

NeedleBall::~NeedleBall()
{
	
}

void NeedleBall::Initialize()
{
	hNeedleBall = Model::Load("Model//Needle.fbx");
	assert(hNeedleBall >= 0);

	hDamageSound = Audio::Load("Sound/Damage.wav"); // ダメージ音の読み込み
	assert(hDamageSound >= 0);
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collider);

}

void NeedleBall::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
	if (transform_.position_.x < -10 || transform_.position_.x > 10 ||
		transform_.position_.z < -10 || transform_.position_.z > 10)
	{
		this->KillMe(); // 画面外に出たら自分を削除
	}
	transform_.rotate_.y += 3.0f;
}

void NeedleBall::Draw()
{
	Model::SetTransform(hNeedleBall, transform_);
	Model::Draw(hNeedleBall);
}

void NeedleBall::Release()
{
}

float NeedleBall::CalculateDistancePlayer(const XMFLOAT3& EnemyPos, const XMFLOAT3& Playerpos)
{
	//2点間の距離を計算
	float dx = EnemyPos.x - Playerpos.x;
	float dy = EnemyPos.y - Playerpos.y;
	float dz = EnemyPos.z - Playerpos.z;

	//距離を返す(計算)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

float NeedleBall::CalculateDistanceWall(const XMFLOAT3& EnemyPos, const XMFLOAT3& Wallpos)
{
	//2点間の距離を計算
	float dx = EnemyPos.x - Wallpos.x;
	float dy = EnemyPos.y - Wallpos.y;
	float dz = EnemyPos.z - Wallpos.z;
	//距離を返す(計算)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

void NeedleBall::OnCollision(GameObject* pTarget)
{
	// 壁との衝突判定
	if (pTarget->GetObjectName() == "Shield")
	{
		moveDirection.x = -moveDirection.x; // 壁に衝突したら反転
		moveDirection.z = -moveDirection.z; // 壁に衝突したら反転
	}
	if (pTarget->GetObjectName() == "Player")
	{
		Audio::Play(hDamageSound); // ダメージ音を再生
	}
	if (pTarget->GetObjectName() == "CannonEnemy")
	{
		Audio::Play(hDamageSound); // ダメージ音を再生
	}
}
