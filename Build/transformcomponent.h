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
	XMFLOAT3 GetDirection(void);
	XMVECTOR GetAxisX(void);
	XMVECTOR GetAxisY(void);
	XMVECTOR GetAxisZ(void);
	XMVECTOR GetQtonX(void);
	XMVECTOR GetQtonY(void);
	XMVECTOR GetQtonZ(void);
	XMMATRIX GetMtxRot(void);
	XMMATRIX GetMtxRotX(void);
	XMMATRIX GetMtxRotY(void);
	XMMATRIX GetMtxRotZ(void);



	XMMATRIX GetWorldMtx(XMMATRIX mtx);
	XMMATRIX GetWorldMtx(void);
	XMMATRIX GetLocalMtx(void);

	void SetPosition(XMFLOAT3 pos);
	void SetRotation(XMFLOAT3 rot);
	void SetScale(XMFLOAT3 scl);
	void SetForward(XMFLOAT3 forward);
	void RotForward(XMFLOAT3 forward);
	void SetAxisX(XMVECTOR axis);
	void SetAxizY(XMVECTOR axis);
	void SetAxizZ(XMVECTOR axis);
	void SetQtonX(XMVECTOR qton);
	void SetQtonY(XMVECTOR qton);
	void SetQtonZ(XMVECTOR qton);
	void SetMtxRot(XMMATRIX mtx);
	void SetMtxRotX(XMMATRIX mtx);
	void SetMtxRotY(XMMATRIX mtx);
	void SetMtxRotZ(XMMATRIX mtx);
	void SetWorldMtx(XMMATRIX mtx);
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

	XMVECTOR		qtonX;		//X����]�̃N�H�[�^�j�I��
	XMVECTOR		qtonY;		//Y����]�̃N�H�[�^�j�I��
	XMVECTOR		qtonZ;		//Z����]�̃N�H�[�^�j�I��

	XMMATRIX		mtxpos;
	XMMATRIX		mtxscl;

	XMMATRIX		mtxrot;			//��]�s��
	XMMATRIX		mtxrotx;		//��]�s��
	XMMATRIX		mtxroty;		//��]�s��
	XMMATRIX		mtxrotz;		//��]�s��


	XMMATRIX		lMtx;
	XMMATRIX		wMtx;	// ���[���h�}�g���b�N�X

	BOOL			isMtxUpdate;

};

