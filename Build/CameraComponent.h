//=============================================================================
//
// �J�������� [camera.h]
// Author : 
//
//=============================================================================
#pragma once
#include "PrimitiveComponent.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


enum VIEWPORT_TYPE
{
	TYPE_FULL_SCREEN,
	TYPE_LEFT_HALF_SCREEN,
	TYPE_RIGHT_HALF_SCREEN,
	TYPE_UP_HALF_SCREEN,
	TYPE_DOWN_HALF_SCREEN,
	TYPE_VIEWPORT,
	TYPE_NONE,

};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class CameraComponent : public PrimitiveComponent
{
public:
	CameraComponent();
	CameraComponent(GameObject* gameObject);
	~CameraComponent();


	void SetCameraAT(XMFLOAT3 pos);
	void SetCameraUp(XMFLOAT3 up);
	void SetLookObject(GameObject* gameObject);


	void SetViewPort(int m_type);
	void GetViewPort(XMFLOAT2& size, XMFLOAT2& pos);
	int GetViewPortType(void);

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Uninit(void) override;

	enum class MODE
	{
		TRACKING,
		TRACKING_SKY,
		
		WORLD,
	};

	void SetMode(MODE mode);

	XMMATRIX GetView(void);


private:
	void SetCamera(void);

	XMFLOAT2 vpSize;
	XMFLOAT2 vpPos;



	XMMATRIX mtxView;

	XMFLOAT3			at;				// �J�����̒����_
	XMFLOAT3			up;				// �J�����̏�����x�N�g��

	float				angle;			// ����p
	float				aspect;			// �A�X�y�N�g��
	float				len;			// �J�����̎��_�ƒ����_�̋���
	float				nearZ;			// �J�����̃N���b�s���O�ŏ��lZ
	float				farZ;			// �J�����̃N���b�s���O�ő�lZ


	GameObject*			lookObject;

	MODE				mode;


};