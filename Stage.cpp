#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/BoxCollider.h"
#include"Player.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"),hFloor(-1),hWall(-1),position({0,0,0})
{
	
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hFloor=Model::Load("Model/Floor.fbx");
	assert(hFloor >= 0);
	hWall= Model::Load("Model/Wall.fbx");
	assert(hWall >= 0);

	//transform_.position_ = { 0,10,5 };
}

void Stage::Update()
{
}

void Stage::Draw()
{
	wallTrans.position_ = { 0,0,0 };
	floorTrans.position_ = { 0,0,0 };
	BoxCollider* collision = new BoxCollider(wallTrans.position_, { 0.2,0.2,0.2 });
	AddCollider(collision);
	for (int z = 0; z < 10; z++)
	{
		for (int x = 0; x < 10; x++)
		{
			if (z == 0 || z == 9 || x == 0 || x == 9)
			{
				wallTrans.position_ = { (float)x,-0.5,(float)z };
				Model::SetTransform(hWall, wallTrans);
				Model::Draw(hWall);
				type = WALL;
			}
			else 
			{
				floorTrans.position_ = { (float)x,-0.5,(float)z };
				Model::SetTransform(hFloor, floorTrans);
				Model::Draw(hFloor);
				type = FLOOR;
			}

		}
	}

	/*Transform floorTrans;
	floorTrans.position_ = { 0,0,0 };
	for (int z = 0; z < 10; z++)
	{
		for (int x = 0; x < 30; x++)
		{
			if (z == 0 || z == 9 || x == 0 || x == 29)continue;
			{
				floorTrans.position_ = { (float)x,-0.5,(float)z };
				Model::SetTransform(hFloor, floorTrans);
				Model::Draw(hFloor);
			}
		}
	}*/
	/*for (int z = 0; z < 10; z++)
	{
		for (int x = 0; x < 20; x++)
		{
			floorTrans.position_ = { (float)x,0,(float)z };
			Model::SetTransform(hModel, floorTrans);
			Model::Draw(hModel);
		}
	}*/
}

void Stage::Release()
{
}

void Stage::OnCollision(GameObject* pTarget)
{
	Player*pPlayer= (Player*)FindObject("Player");
	if (pTarget->GetObjectName() == "Player")
	{
		if (type == WALL)
		{
			pPlayer->KillMe();
		}
	}
}



