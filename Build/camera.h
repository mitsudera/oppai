#pragma once
#include "gameobject.h"

class CameraComponent;

class Camera :public GameObject
{
public:
	Camera(Scene* scene);
	Camera(GameObject* gameObject);
	~Camera();

	virtual void Init(void) override;

	CameraComponent* GetCameraComponent(void);

private:
	CameraComponent* cameraComponent;

};

