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
	hModel=Model::Load("Model/Floor.fbx");
	assert(hModel >= 0);
	transform_.position_ = { 0,10,5 };
}

void Stage::Update()
{
}

void Stage::Draw()
{
	Transform floorTrans;
	floorTrans.position_ = { 0,0,0 };
	for (int z = 0; z < 20; z++)
	{
		for (int x = 0; x < 20; x++)
		{
			floorTrans.position_ = { (float)x,-0.5,(float)z };
			Model::SetTransform(hModel, floorTrans);
			Model::Draw(hModel);
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



