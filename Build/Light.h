#pragma once
#include "gameobject.h"

class DirectionalLightComponent;


class Light :public GameObject
{
public:

	Light(Level* level);
	~Light();



	// GameObject ����Čp������܂���
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;
	
private:

	DirectionalLightComponent* dirLight;

};
