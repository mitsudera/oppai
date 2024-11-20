#pragma once
#include "component.h"
#include "Coreminimal.h"



class MissileComponent :public Component
{
public:
	MissileComponent(GameObject* gameObject);
	~MissileComponent();
	// Component ����Čp������܂���
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void SetSpeed(float f);
	void SetSpeedMax(float f);
	void SetSpeedUp(float f);
	void SetSpeedUpMax(float f);
	void SetSpeedUpValue(float f);
	void SetRotValue(float f);

	void Launch(XMFLOAT3 pos, XMMATRIX rot, float spd, GameObject* target);
	BOOL GetUse(void);

private:

	GameObject* target;
	float			spd;			// �ړ��X�s�[�h
	float			spdmax;			//�ō����x
	float			spdup;			//����
	float			spdupmax;		//�����x�}�b�N�X
	float			spdupvalue;		//�����x
	float			rotValue;
	//BOOL use;
	int life;


};

