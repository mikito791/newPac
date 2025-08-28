#include "Ghost.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Debug.h"
#include"Engine/SceneManager.h"

Ghost::Ghost(GameObject* parent)
	: GameObject(parent, "Ghost")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // 乱数初期化（毎回違う結果にする）
	hGhost = -1; // モデルハンドルの初期化
}

Ghost::~Ghost()
{
}

void Ghost::Initialize()
{
	hGhost = Model::Load("Model/Ghost.fbx");
	assert(hGhost >= 0);
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collider);
}

void Ghost::Update()
{
	transform_.position_.x += moveDirection.x;
	transform_.position_.z += moveDirection.z;
	if (transform_.position_.x < -10 || transform_.position_.x > 10 ||
		transform_.position_.z < -10 || transform_.position_.z > 10)
	{
		this->KillMe(); // 画面外に出たら自分を削除
	}
}

void Ghost::Draw()
{
	Model::SetTransform(hGhost, transform_);
	Model::Draw(hGhost);
}

void Ghost::Release()
{
}

void Ghost::OnCollision(GameObject* pTarget)
{
}
