//=============================================================================
//
// �p�[�e�B�N������ [cloud.h]
// Author : 
//
//=============================================================================
#pragma once
#include "gameobject.h"

class CloudComponent;

class Cloud :public GameObject
{

public:
	Cloud(Level* level);
	~Cloud();
	// GameObject ����Čp������܂���
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

private:

	CloudComponent* cloudComponent;
};

