#include "Bomb.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include <cstdlib> // for rand()
#include<ctime>
#include<cmath>
#include"Engine/Audio.h"
#include"Engine/VFX.h"


Bomb::Bomb(GameObject* parent)
	:GameObject(parent, "Bomb")
{
	hBomb = -1;
	hBombSound = -1;
}

Bomb::~Bomb()
{
}

void Bomb::Initialize()
{
	hBomb = Model::Load("Model/Bomb.fbx");
	assert(hBomb >= 0);
	hBombSound = Audio::Load("Sound/Bomb.wav"); // 爆弾音の読み込み
	assert(hBombSound >= 0);
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(collider);
}

void Bomb::Update()
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

void Bomb::Draw()
{
	Model::SetTransform(hBomb, transform_);
	Model::Draw(hBomb);
}

void Bomb::Release()
{
	if (hBomb >= 0)
	{
		Model::Release(hBomb);
		hBomb = -1;
	}
}

void Bomb::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Shield")
	{
		this->KillMe(); // 壁に衝突したら自分を削除
		BombEffect(); // 爆発エフェクトを生成
		Audio::Play(hBombSound); // 爆弾音を再生
	}
	if (pTarget->GetObjectName() == "Player")
	{
		this->KillMe();
		BombEffect();
		Audio::Play(hBombSound); // 爆弾音を再生
	}
}

void Bomb::BombEffect()
{
	EmitterData data;

	//炎
	data.textureFileName = "PaticleAssets//cloudA.png";
	data.position = transform_.position_;
	data.delay = 0;
	data.number = 80;
	data.lifeTime = 30;
	data.direction = XMFLOAT3(0, 1, 0);
	data.directionRnd = XMFLOAT3(90, 90, 90);
	data.speed = 0.1f;
	data.speedRnd = 0.8;
	data.size = XMFLOAT2(1.2, 1.2);
	data.sizeRnd = XMFLOAT2(0.4, 0.4);
	data.scale = XMFLOAT2(1.05, 1.05);
	data.color = XMFLOAT4(1, 1, 0.1, 1);
	data.deltaColor = XMFLOAT4(0, -1.0 / 20, 0, -1.0 / 20);
	VFX::Start(data);

	//火の粉
	data.delay = 0;
	data.number = 80;
	data.lifeTime = 100;
	data.positionRnd = transform_.position_;
	data.direction = XMFLOAT3(0, 1, 0);
	data.directionRnd = XMFLOAT3(90, 90, 90);
	data.speed = 0.25f;
	data.speedRnd = 1;
	data.accel = 0.93;
	data.size = XMFLOAT2(0.1, 0.1);
	data.sizeRnd = XMFLOAT2(0.4, 0.4);
	data.scale = XMFLOAT2(0.99, 0.99);
	data.color = XMFLOAT4(1, 1, 0.1, 1);
	data.deltaColor = XMFLOAT4(0, 0, 0, 0);
	data.gravity = 0.003f;
	VFX::Start(data);

	//地面
	data.textureFileName = "PaticleAssets//flashA_R.png";
	data.positionRnd = transform_.position_;
	data.isBillBoard = false;
	data.rotate.x = 90;
	data.delay = 0;
	data.number = 1;
	data.lifeTime = 7;
	data.speed = 0;
	data.size = XMFLOAT2(5, 5);
	data.sizeRnd = XMFLOAT2(0, 0);
	data.scale = XMFLOAT2(1.25f, 1.25f);
	data.color = XMFLOAT4(1, 1, 1, 0.3f);
	VFX::Start(data);
}
