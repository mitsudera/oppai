#pragma once
#include "primitivecomponent.h"
#include "ShaderSet.h"
class MeshFieldComponent :public PrimitiveComponent
{
public:
	MeshFieldComponent(GameObject* gameObject);
	~MeshFieldComponent();
	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;

	virtual void Draw(void) override;

	void CreateVetexBuffer(XMFLOAT2 uvScale, XMFLOAT2 scale, XMFLOAT4 color);

private:

	VERTEX_3D vertexArray[4];
	ID3D11Buffer* vertexBuffer;

};

