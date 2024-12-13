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

	void UpdateMatrix(void);//描画処理の直前で行列を更新

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

	//このトランスフォームのX軸方向に動かす
	void MoveXAxis(float f);
	//このトランスフォームのY軸方向に動かす
	void MoveYAxis(float f);
	//このトランスフォームのZ軸方向に動かす
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

	

	XMFLOAT3		pos;		// ポリゴンの位置
	XMFLOAT3		oldPos;
	XMFLOAT3		rot;		// ポリゴンの向き
	XMFLOAT3		oldRot;
	XMFLOAT3		scl;		// ポリゴンの大きさ(スケール)
	XMFLOAT3		forward;
	XMFLOAT3		fDirection;

	XMVECTOR		axisX;
	XMVECTOR		axisY;
	XMVECTOR		axisZ;


	XMMATRIX		mtxpos;
	XMMATRIX		mtxscl;

	XMMATRIX		mtxrot;			//回転行列


	XMMATRIX		lMtx;

	BOOL			isMtxUpdate;

};

