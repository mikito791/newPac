#include "Stage.h"
#include "Engine/Model.h"
Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
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
	Transform wallTrans;
	wallTrans.position_ = { 0,0,0 };
	for (int z = 0; z < 10; z++)
	{
		for (int x = 0; x < 30; x++)
		{
			if (z == 0 || z == 9 || x == 0 || x == 29)
			{
				wallTrans.position_ = { (float)x,-0.5,(float)z };
				Model::SetTransform(hWall, wallTrans);
				Model::Draw(hWall);
			}
		}
	}

	Transform floorTrans;
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
	}
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



