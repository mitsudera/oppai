#include "TitleLogo.h"
#include "SpriteComponent.h"
TitleLogo::TitleLogo(Scene* scene)
{
	this->pScene = scene;
}

TitleLogo::~TitleLogo()
{
}

void TitleLogo::Init(void)
{
	GameObject::Init();
	this->layer = Layer::UI;

	this->name = "TitleLogo";
	SpriteComponent* logo = new SpriteComponent(this);
	logo->Init();
	logo->CreateVertexBuffer();
	logo->SetSpriteLeftTop("data/texture/bg000.jpg", XMFLOAT3(0.0f, 0.0f, 0.0f), 1920.0f, 1080.0f);
	this->componentList.push_back(logo);


}
