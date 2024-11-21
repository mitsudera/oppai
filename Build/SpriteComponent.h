#pragma once
#include "primitivecomponent.h"

class SpriteComponent : public PrimitiveComponent
{
public:
	SpriteComponent(GameObject* gameObject);
	~SpriteComponent();

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;
	virtual void Draw(void) override;

	void CreateVertexBuffer(void);

	void SetSpriteCenter(string texPath, XMFLOAT3 pos, float width, float height);
	void SetSpriteLeftTop(string texPath, XMFLOAT3 pos, float width, float height);

private:
	int texIndex;
	ID3D11Buffer* vertexBuffer;


};

