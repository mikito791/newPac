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

NeedleBall::NeedleBall(GameObject* parent)
	: GameObject(parent, "NeedleBall")
{
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // óêêîèâä˙âªÅiñàâÒà·Ç§åãâ Ç…Ç∑ÇÈÅj
	speed = 0.05f; // à⁄ìÆë¨ìx
	distance = 0.001f; // è’ìÀîªíËÇÃãóó£
}

NeedleBall::~NeedleBall()
{
	
}

void NeedleBall::Initialize()
{
	hNeedleBall = Model::Load("Model//Needle.fbx");
	assert(hNeedleBall >= 0);
	
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
		this->KillMe(); // âÊñ äOÇ…èoÇΩÇÁé©ï™ÇçÌèú
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
	//2ì_ä‘ÇÃãóó£ÇåvéZ
	float dx = EnemyPos.x - Playerpos.x;
	float dy = EnemyPos.y - Playerpos.y;
	float dz = EnemyPos.z - Playerpos.z;

	//ãóó£Çï‘Ç∑(åvéZ)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

float NeedleBall::CalculateDistanceWall(const XMFLOAT3& EnemyPos, const XMFLOAT3& Wallpos)
{
	//2ì_ä‘ÇÃãóó£ÇåvéZ
	float dx = EnemyPos.x - Wallpos.x;
	float dy = EnemyPos.y - Wallpos.y;
	float dz = EnemyPos.z - Wallpos.z;
	//ãóó£Çï‘Ç∑(åvéZ)
	float distance = sqrt(dx * dx + dy * dy + dz * dz);
	return distance;
}

void NeedleBall::OnCollision(GameObject* pTarget)
{
	// ï«Ç∆ÇÃè’ìÀîªíË
	if (pTarget->GetObjectName() == "Shield")
	{
		moveDirection.x = -moveDirection.x; // ï«Ç…è’ìÀÇµÇΩÇÁîΩì]
		moveDirection.z = -moveDirection.z; // ï«Ç…è’ìÀÇµÇΩÇÁîΩì]
	}
}
