#pragma once
#include "component.h"

class GameObject;

class JetEngineComponent :public Component
{
public:

	JetEngineComponent(GameObject* gameObject);
	~JetEngineComponent();


	// Component ����Čp������܂���
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void SetSpeed(float f);
	float GetSpeed(void);
	void SetSpeedMax(float f);
	float GetSpeedMax(void);
	void SetSpeedUp(float f);
	float GetSpeedUp(void);
	void SetSpeedUpMax(float f);
	float GetSpeedUpMax(void);
	void SetSpeedUpValue(float f);
	float GetSpeedUpValue(void);
	void SetBrake(float f);
	float GetBrake(void);

	void Brake(void);
	void Accel(void);


private:

	float			spd;			// �ړ��X�s�[�h
	float			spdmax;			//�ō����x
	float			spdup;			//����
	float			spdupmax;		//�����x�}�b�N�X
	float			spdupvalue;		//�����x
	float			brake;			//�u���[�L



};

