#pragma once
#include "component.h"
#include "Coreminimal.h"

class TransformComponent:public Component
{
public:




	TransformComponent();
	TransformComponent(GameObject* gameObject);
	~TransformComponent();

	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

	void UpdateMatrix(void);//�`�揈���̒��O�ōs����X�V

	virtual XMFLOAT3 GetPosition(void);


	XMFLOAT3 GetOldPosition(void);
	XMFLOAT3 GetRotation(void);
	XMFLOAT3 GetOldRotation(void);
	XMFLOAT3 GetScale(void);
	XMFLOAT3 GetForward(void);
	XMVECTOR GetAxisX(void);
	XMVECTOR GetAxisY(void);
	XMVECTOR GetAxisZ(void);
	XMMATRIX GetMtxRot(void);



	XMMATRIX GetWorldMtx(XMMATRIX mtx);
	XMMATRIX GetWorldMtx(void);
	XMMATRIX GetLocalMtx(void);

	void SetPosition(XMFLOAT3 pos);
	void SetRotation(XMFLOAT3 rot);
	void SetScale(XMFLOAT3 scl);
	void RotForward(XMFLOAT3 forward);
	void SetMtxRot(XMMATRIX mtx);
	void SetLocalMtx(XMMATRIX mtx);
	void SetTransForm(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl);



	XMFLOAT3 GetWorldPos(void);

	void MoveX(float f);
	void MoveY(float f);
	void MoveZ(float f);

	//���̃g�����X�t�H�[����X�������ɓ�����
	void MoveXAxis(float f);
	//���̃g�����X�t�H�[����Y�������ɓ�����
	void MoveYAxis(float f);
	//���̃g�����X�t�H�[����Z�������ɓ�����
	void MoveZAxis(float f);

	void MoveForward(float f);


	void PosUpdate(void);

	void SetForwardDiection(XMFLOAT3 dir);

	void RotRoll(float f);
	void RotPitch(float f);
	void RotYaw(float f);

	void RotWorldRoll(float f);

	void RotWorldPitch(float f);

	void RotWorldYaw(float f);

	void Rotate(float f, XMVECTOR axis);

	void RotAxis(XMVECTOR qton);

	void RotAxisAngle(XMVECTOR axis, float angle);
	
	float Length(TransformComponent* transform);

	void SetMtxUpdate(BOOL flag);


protected:

	

	XMFLOAT3		pos;		// �|���S���̈ʒu
	XMFLOAT3		oldPos;
	XMFLOAT3		rot;		// �|���S���̌���
	XMFLOAT3		oldRot;
	XMFLOAT3		scl;		// �|���S���̑傫��(�X�P�[��)
	XMFLOAT3		forward;
	XMFLOAT3		fDirection;

	XMVECTOR		axisX;
	XMVECTOR		axisY;
	XMVECTOR		axisZ;


	XMMATRIX		mtxpos;
	XMMATRIX		mtxscl;

	XMMATRIX		mtxrot;			//��]�s��


	XMMATRIX		lMtx;

	BOOL			isMtxUpdate;

};

