#include "CannonEnemy.h"
#include "Engine/Model.h"

enum EnemyID
{
	Left,
	Right,
	Back,
	Front,
};
CannonEnemy::CannonEnemy(GameObject* parent)
{
	hCannonEnemy = -1; // ���f���n���h���̏�����
	EnemyHP = 3; // �G��HP�̏����l�ݒ�
	EnemyId[0] = Left; // �G��ID��ݒ�
	EnemyId[1] = Right;
	EnemyId[2] = Back;
	EnemyId[3] = Front;
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
		case 0: // ������
			transform_.position_ = XMFLOAT3(-2.5, 0, 2);
			transform_.rotate_.y = 90; // ������̌���
			Model::SetTransform(hCannonEnemy, transform_);
			Model::Draw(hCannonEnemy);
			break;
		case 1: // �E����
			transform_.position_ = XMFLOAT3(10.5, 0, 2);
			transform_.rotate_.y = 270; // �E����̌���
			Model::SetTransform(hCannonEnemy, transform_);
			Model::Draw(hCannonEnemy);
			break;
		case 2: // ������
			transform_.position_ = XMFLOAT3(4, 0, 7);
			transform_.rotate_.y = 180; // ������̌���
			Model::SetTransform(hCannonEnemy, transform_);
			Model::Draw(hCannonEnemy);
			break;
		case 3: // ��O����
			transform_.position_ = XMFLOAT3(4, 0, -1.5);
			transform_.rotate_.y = 0; // ��O����̌���
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
		pTarget->KillMe(); // Needle�ɓ��������玩�����폜
	}
}

void CannonEnemy::HpDown(int hp)
{
	EnemyHP -= hp; // HP�����炷
}
