#pragma once
#include "component.h"
#include "Coreminimal.h"

#define LIGHT_MAX		(8)

class LightComponent :public Component
{
public:

	LightComponent();
	LightComponent(GameObject* gameObject);
	~LightComponent();

	
	

	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void SetIndex(int n);
	int GetIndex(void);

	void SetDiffuse(XMFLOAT4 diffuse);
	XMFLOAT4 GetDiffuse(void);

	void SetEnable(BOOL enable);
	BOOL GetEnable(void);

	XMFLOAT3 GetDirection(void);
	void SetDirecton(XMFLOAT3 forward);
	XMFLOAT3 GetPosition(void);
	void SetPosition(XMFLOAT3 pos);
	XMFLOAT4 GetAmbient(void);
	void SetAmbient(XMFLOAT4 amb);
	float GetAttenuation(void);
	void SetAttenuation(float att);

	BOOL GetType(void);

	void Set(void);

	enum LIGHT_TYPE
	{
		NOT,
		DIRECTIONAL,
		POINT,
	};

protected:

	int index;				//���Ԗڂ̃��C�g�o�b�t�@�ɓo�^���邩�H

	XMFLOAT3	direction;	// ���C�g�̕���
	XMFLOAT3	position;	// ���C�g�̈ʒu
	XMFLOAT4	diffuse;	// �g�U���̐F
	XMFLOAT4	ambient;	// �����̐F
	float		attenuation;// ������
	int			type;		// ���C�g��ʃt���O
	BOOL		enable;		// �L���t���O


};

