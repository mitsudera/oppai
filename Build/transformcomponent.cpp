#include "transformcomponent.h"
#include "math.h"
#include"gameobject.h"

TransformComponent::TransformComponent()
{
	this->attribute = Attribute::Transform;
	this->pos = { 0.0f,0.0f,0.0f };
	this->oldPos = { 0.0f, 0.0f, 0.0f };
	this->rot = { 0.0f,0.0f,0.0f };
	this->oldRot = { 0.0f,0.0f,0.0f };
	this->scl = { 1.0f,1.0f,1.0f };
	this->forward = { 0.0f,0.0f,-1.0f };
	this->axisX = xonevec();
	this->axisY = yonevec();
	this->axisZ = zonevec();

	this->mtxpos = XMMatrixIdentity();
	this->mtxscl = XMMatrixIdentity();
	this->mtxrot = XMMatrixIdentity();
	this->lMtx = XMMatrixIdentity();
	this->fDirection = { 0.0f,0.0f,1.0f };
	isMtxUpdate = TRUE;
}

TransformComponent::TransformComponent(GameObject* gameObject)
{
	attribute = Attribute::Transform;
	this->pos = { 0.0f,0.0f,0.0f };
	this->oldPos = { 0.0f, 0.0f, 0.0f };
	this->rot = { 0.0f,0.0f,0.0f };
	this->oldRot = { 0.0f,0.0f,0.0f };
	this->scl = { 1.0f,1.0f,1.0f };
	this->forward = { 0.0f,0.0f,1.0f };
	this->axisX = xonevec();
	this->axisY = yonevec();
	this->axisZ = zonevec();

	this->mtxpos = XMMatrixIdentity();
	this->mtxscl = XMMatrixIdentity();
	this->mtxrot = XMMatrixIdentity();
	this->lMtx = XMMatrixIdentity();
	this->fDirection = { 0.0f,0.0f,1.0f };

	this->pGameObject = gameObject;
	isMtxUpdate = TRUE;

}

TransformComponent::~TransformComponent()
{


}
void TransformComponent::Init(void)
{
	Component::Init();

	attribute = Attribute::Transform;


	this->pos = { 0.0f,0.0f,0.0f };
	this->oldPos = { 0.0f, 0.0f, 0.0f };
	this->rot = { 0.0f,0.0f,0.0f };
	this->oldRot = { 0.0f,0.0f,0.0f };
	this->scl = { 1.0f,1.0f,1.0f };
	this->forward = { 0.0f,0.0f,1.0f };
	this->fDirection= { 0.0f,0.0f,1.0f };
	this->axisX = xonevec();
	this->axisY = yonevec();
	this->axisZ = zonevec();

	this->mtxpos = XMMatrixIdentity();
	this->mtxscl = XMMatrixIdentity();
	this->mtxrot = XMMatrixIdentity();
	this->lMtx = XMMatrixIdentity();
	isMtxUpdate = TRUE;

}

void TransformComponent::Uninit(void)
{
	Component::Uninit();
}

void TransformComponent::Update(void)
{
	Component::Update();


	XMVECTOR v = XMLoadFloat3(&fDirection);

	v = XMVector3Transform(v, mtxrot);

	v = XMVector3Normalize(v);

	XMStoreFloat3(&this->forward, v);

	
	

	this->oldPos = pos;
	this->oldRot = rot;


}



void TransformComponent::Draw(void)
{
	Component::Draw();

}

void TransformComponent::UpdateMatrix(void)
{
	XMVECTOR v = XMLoadFloat3(&fDirection);

	v = XMVector3Transform(v, mtxrot);

	v = XMVector3Normalize(v);

	XMStoreFloat3(&this->forward, v);




	if (!this->isMtxUpdate)
		return;
	lMtx = XMMatrixIdentity();
	lMtx = XMMatrixMultiply(lMtx, mtxscl);
	lMtx = XMMatrixMultiply(lMtx, mtxrot);
	lMtx = XMMatrixMultiply(lMtx, mtxpos);

}

XMFLOAT3 TransformComponent::GetPosition(void)
{
	return this->pos;
}

XMFLOAT3 TransformComponent::GetOldPosition(void)
{
	return this->oldPos;
}

XMFLOAT3 TransformComponent::GetRotation(void)
{
	return this->rot;
}

XMFLOAT3 TransformComponent::GetOldRotation(void)
{
	return this->oldRot;
}

XMFLOAT3 TransformComponent::GetScale(void)
{
	return this->scl;
}

XMFLOAT3 TransformComponent::GetForward(void)
{
	return this->forward;
}

XMVECTOR TransformComponent::GetAxisX(void)
{
	return this->axisX;
}

XMVECTOR TransformComponent::GetAxisY(void)
{
	return this->axisY;
}

XMVECTOR TransformComponent::GetAxisZ(void)
{
	return this->axisZ;
}


XMMATRIX TransformComponent::GetMtxRot(void)
{
	return this->mtxrot;
}

XMMATRIX TransformComponent::GetWorldMtx(XMMATRIX mtx)
{
	XMMATRIX ans = XMMatrixMultiply(mtx,lMtx);


	if (pGameObject->GetParent() != nullptr)
	{
		ans = pGameObject->GetParent()->GetTransFormComponent()->GetWorldMtx(ans);

	}
	return ans;
}

XMMATRIX TransformComponent::GetWorldMtx(void)
{
	return GetWorldMtx(XMMatrixIdentity());
}

XMMATRIX TransformComponent::GetLocalMtx(void)
{
	return this->lMtx;
}


void TransformComponent::SetPosition(XMFLOAT3 pos)
{
	this->pos = pos;
	this->mtxpos = XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void TransformComponent::SetRotation(XMFLOAT3 rot)
{
	this->rot = rot;
	

}

void TransformComponent::SetScale(XMFLOAT3 scl)
{
	this->scl = scl;
	this->mtxscl = XMMatrixScaling(scl.x, scl.y, scl.z);
}


void TransformComponent::RotForward(XMFLOAT3 forward)
{
	XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&forward));
	XMVECTOR d = XMVector3Normalize(XMLoadFloat3(&this->forward));

	float dot;
	XMStoreFloat(&dot, XMVector3Dot(f, d));

	if (dot == -1.0f)
	{

		XMVECTOR qton = XMQuaternionRotationAxis(axisX, XM_PI);
		this->RotAxis(qton);
		mtxrot = XMMatrixRotationQuaternion(qton);
		return;
	}
	if (XMVector3Equal(f, d))
	{
		return;
	}

	XMVECTOR c = XMVector3Cross(f, d);

	XMVECTOR a = XMVector3AngleBetweenNormals(f, d);
	float angle;
	XMStoreFloat(&angle, a);



	XMVECTOR q = XMQuaternionRotationAxis(c, angle);
	XMMATRIX mtx = XMMatrixRotationQuaternion(q);

	this->RotAxis(q);

	this->mtxrot = mtx;

}


void TransformComponent::SetMtxRot(XMMATRIX mtx)
{
	this->mtxrot = mtx;
}


void TransformComponent::SetLocalMtx(XMMATRIX mtx)
{
	this->lMtx = mtx;
}

void TransformComponent::SetTransForm(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl)
{
	this->SetPosition(pos);
	this->SetRotation(rot);
	this->SetScale(scl);
}

XMFLOAT3 TransformComponent::GetWorldPos(void)
{
	XMFLOAT3 lPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMVECTOR wPos = XMLoadFloat3(&lPos);
	wPos = XMVector3Transform(wPos,GetWorldMtx());

	XMStoreFloat3(&lPos, wPos);
	XMMATRIX mtx = GetWorldMtx();

	return lPos;


}

void TransformComponent::MoveX(float f)
{
	this->pos.x += f;
	this->PosUpdate();

}

void TransformComponent::MoveY(float f)
{
	this->pos.y += f;
	this->PosUpdate();

}

void TransformComponent::MoveZ(float f)
{
	this->pos.z += f;
	this->PosUpdate();

}

void TransformComponent::MoveXAxis(float f)
{

	XMFLOAT3 a;

	XMStoreFloat3(&a, this->axisX);

	MoveX(a.x*f);
	MoveY(a.y*f);
	MoveZ(a.z*f);

}

void TransformComponent::MoveYAxis(float f)
{
	XMFLOAT3 a;

	XMStoreFloat3(&a, this->axisY);

	MoveX(a.x * f);
	MoveY(a.y * f);
	MoveZ(a.z * f);

}

void TransformComponent::MoveZAxis(float f)
{
	XMFLOAT3 a;

	XMStoreFloat3(&a, this->axisZ);

	MoveX(a.x * f);
	MoveY(a.y * f);
	MoveZ(a.z * f);

}


void TransformComponent::MoveForward(float f)
{
	float x, y, z;
	x = this->forward.x * f;
	y = this->forward.y * f;
	z = this->forward.z * f;

	MoveX(x);
	MoveY(y);
	MoveZ(z);


}

void TransformComponent::PosUpdate(void)
{
	this->mtxpos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
}

void TransformComponent::SetForwardDiection(XMFLOAT3 dir)
{
	this->fDirection = XMFLOAT3Normalize(dir);
	this->forward = XMFLOAT3Normalize(dir);
}




void TransformComponent::RotRoll(float f)
{
	Rotate(f, axisZ);
}

void TransformComponent::RotPitch(float f)
{
	Rotate(f, axisX);
}

void TransformComponent::RotYaw(float f)
{
	
	Rotate(f, axisY);
}

void TransformComponent::RotWorldRoll(float f)
{
	Rotate(f, zonevec());
}

void TransformComponent::RotWorldPitch(float f)
{
	Rotate(f, xonevec());
}

void TransformComponent::RotWorldYaw(float f)
{
	
	Rotate(f, yonevec());
}

void TransformComponent::Rotate(float f, XMVECTOR axis)
{
	XMVECTOR qton = XMQuaternionRotationAxis(axis, f);
	qton = XMQuaternionNormalize(qton); // クォータニオンを正規化
	this->mtxrot = XMMatrixMultiply(this->mtxrot, XMMatrixRotationQuaternion(qton));
	RotAxis(qton);
}



void TransformComponent::RotAxis(XMVECTOR qton)
{
	this->axisX = XMVector3Rotate(this->axisX, qton);
	this->axisX = XMVector3Normalize(this->axisX);
	this->axisY = XMVector3Rotate(this->axisY, qton);
	this->axisY = XMVector3Normalize(this->axisY);
	this->axisZ = XMVector3Rotate(this->axisZ, qton);
	this->axisZ = XMVector3Normalize(this->axisZ);

}




void TransformComponent::RotAxisAngle(XMVECTOR axis, float angle)
{
	XMVECTOR qton = XMQuaternionRotationAxis(axis, angle);
	RotAxis(qton);

	mtxrot = XMMatrixMultiply(mtxrot, XMMatrixRotationQuaternion(qton));
}

float TransformComponent::Length(TransformComponent* transform)
{
	return XMFLOAT3Length(this->GetWorldPos(), transform->GetWorldPos());
}

void TransformComponent::SetMtxUpdate(BOOL flag)
{
	this->isMtxUpdate = flag;
}
