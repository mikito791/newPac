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
	hCannonEnemy = -1; // ���f���n���h���̏�����
	EnemyHP = 3; // �G��HP�̏����l�ݒ�
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
		HpDown(1); // Needle�ɓ���������HP��1���炷
		pTarget->KillMe(); // Needle�ɓ��������玩�����폜
		if (EnemyHP <= 0)
		{
			EnemyHP = 0; // HP��0�����ɂȂ�Ȃ��悤�ɐ���
			this->KillMe(); // HP��0�ȉ��ɂȂ����玩�����폜
		}
	}
}

void CannonEnemy::HpDown(int hp)
{
	EnemyHP -= hp; // HP�����炷
}
