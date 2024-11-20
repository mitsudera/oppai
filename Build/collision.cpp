//=============================================================================
//
// �����蔻�菈�� [collision.cpp]
// Author : 
//
//=============================================================================
#include "collision.h"

//=============================================================================
// ����(dot)
//=============================================================================
float dotProduct(XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	float ans = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
#else
	// �_�C���N�g�w�ł́A�A�A
	XMVECTOR temp = XMVector3Dot(*v1, *v2);
	float ans = 0.0f;
	XMStoreFloat(&ans, temp);
#endif

	return(ans);
}


//=============================================================================
// �O��(cross)
//=============================================================================
void crossProduct(XMVECTOR* ret, XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	ret->x = v1->y * v2->z - v1->z * v2->y;
	ret->y = v1->z * v2->x - v1->x * v2->z;
	ret->z = v1->x * v2->y - v1->y * v2->x;
#else
	// �_�C���N�g�w�ł́A�A�A
	* ret = XMVector3Cross(*v1, *v2);
#endif

}


//=============================================================================
// ���C�L���X�g
// p0, p1, p2�@�|���S���̂R���_
// pos0 �n�_
// pos1 �I�_
// hit�@��_�̕ԋp�p
// normal �@���x�N�g���̕ԋp�p
// �������Ă���ꍇ�ATRUE��Ԃ�
//=============================================================================
BOOL RayCast(XMFLOAT3 xp0, XMFLOAT3 xp1, XMFLOAT3 xp2, XMFLOAT3 xpos0, XMFLOAT3 xpos1, XMFLOAT3* hit, XMFLOAT3* normal)
{
	XMVECTOR	p0 = XMLoadFloat3(&xp0);
	XMVECTOR	p1 = XMLoadFloat3(&xp1);
	XMVECTOR	p2 = XMLoadFloat3(&xp2);
	XMVECTOR	pos0 = XMLoadFloat3(&xpos0);
	XMVECTOR	pos1 = XMLoadFloat3(&xpos1);

	XMVECTOR	nor;	// �|���S���̖@��
	XMVECTOR	vec1;
	XMVECTOR	vec2;
	float		d1, d2;

	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�(���̏����͌Œ蕨�Ȃ�\��Init()�ōs���Ă����Ɨǂ�)
		vec1 = p1 - p0;
		vec2 = p2 - p0;
		crossProduct(&nor, &vec2, &vec1);
		nor = XMVector3Normalize(nor);		// �v�Z���₷���悤�ɖ@�����m�[�}���C�Y���Ă���(���̃x�N�g���̒������P�ɂ��Ă���)
		XMStoreFloat3(normal, nor);			// ���߂��@�������Ă���
	}

	// �|���S�����ʂƐ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�i�s�p�Ȃ�{�A�݊p�Ȃ�[�A���p�Ȃ�O�j
	vec1 = pos0 - p0;
	vec2 = pos1 - p0;
	{	// ���߂��|���S���̖@���ƂQ�̃x�N�g���i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		d1 = dotProduct(&vec1, &nor);
		d2 = dotProduct(&vec2, &nor);
		if (((d1 * d2) > 0.0f) || (d1 == 0 && d2 == 0))
		{
			// �������Ă���\���͖���
			return(FALSE);
		}
	}


	{	// �|���S���Ɛ����̌�_�����߂�
		d1 = (float)fabs(d1);	// ��Βl�����߂Ă���
		d2 = (float)fabs(d2);	// ��Βl�����߂Ă���
		float a = d1 / (d1 + d2);							// ������

		XMVECTOR	vec3 = (1 - a) * vec1 + a * vec2;		// p0�����_�ւ̃x�N�g��
		XMVECTOR	p3 = p0 + vec3;							// ��_
		XMStoreFloat3(hit, p3);								// ���߂���_�����Ă���

		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�

			// �|���S���̊e�ӂ̃x�N�g��
			XMVECTOR	v1 = p1 - p0;
			XMVECTOR	v2 = p2 - p1;
			XMVECTOR	v3 = p0 - p2;

			// �e���_�ƌ�_�Ƃ̃x�N�g��
			XMVECTOR	v4 = p3 - p1;
			XMVECTOR	v5 = p3 - p2;
			XMVECTOR	v6 = p3 - p0;

			// �O�ςŊe�ӂ̖@�������߂āA�|���S���̖@���Ƃ̓��ς��Ƃ��ĕ������`�F�b�N����
			XMVECTOR	n1, n2, n3;

			crossProduct(&n1, &v4, &v1);
			if (dotProduct(&n1, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�

			crossProduct(&n2, &v5, &v2);
			if (dotProduct(&n2, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�

			crossProduct(&n3, &v6, &v3);
			if (dotProduct(&n3, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�
		}
	}

	return(TRUE);	// �������Ă���I(hit�ɂ͓������Ă����_�������Ă���Bnormal�ɂ͖@���������Ă���)
}

BOOL CollisionPointSphere(XMFLOAT3 point, XMFLOAT3 center, float r)
{
	BOOL ans = FALSE;

	XMVECTOR p = XMLoadFloat3(&point);
	XMVECTOR cp = XMLoadFloat3(&center);
	XMVECTOR lv = p - cp;

	float len;
	XMStoreFloat(&len, XMVector3Length(lv));

	if (len<r)
	{
		ans = TRUE;
	}
	return ans;
}
BOOL CollisionPointBox(XMFLOAT3 point, XMMATRIX boxMatrix)
{
	return FALSE;
}

BOOL CollisionPointCapsule(XMFLOAT3 point, XMFLOAT3 pos1, XMFLOAT3 pos2,float r)
{
	BOOL ans = FALSE;

	XMVECTOR p = XMLoadFloat3(&point);

	XMVECTOR p1 = XMLoadFloat3(&pos1);
	XMVECTOR p2 = XMLoadFloat3(&pos2);

	XMVECTOR p1c = p - p1;
	XMVECTOR p2c = p - p2;
	float p1l;
	float p2l;
	XMStoreFloat(&p1l, XMVector3Length(p1c));
	XMStoreFloat(&p2l, XMVector3Length(p2c));


	//�����̗��[���܂��͌���
	if (p1l < r || p2l < r)
	{
		return TRUE;
	}

	XMVECTOR lv = XMVector3LinePointDistance(p1, p2, p);
	float length;
	XMStoreFloat(&length, lv);
	if (length < r)
	{
		return TRUE;
	}

	return ans;
}





//�����Ƌ��̓����蔻��
BOOL CollisionLineSphere(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 center ,float r)
{
	BOOL ans = FALSE;
	XMVECTOR p1 = XMLoadFloat3(&pos1);
	XMVECTOR p2 = XMLoadFloat3(&pos2);
	XMVECTOR cp = XMLoadFloat3(&center);

	XMVECTOR p1c = cp - p1;
	XMVECTOR p2c = cp - p2;
	float p1l;
	float p2l;
	XMStoreFloat(&p1l, XMVector3Length(p1c));
	XMStoreFloat(&p2l, XMVector3Length(p2c));


	//�����̗��[���܂��͌���
	if (p1l < r || p2l < r)
	{
		return TRUE;
	}

	XMVECTOR lv = XMVector3LinePointDistance(p1,p2,cp);
	float length;
	XMStoreFloat(&length, lv);
	if (length<r)
	{
		return TRUE;
	}

	return ans;

}

BOOL CollisionLineCapsule(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 cp1, XMFLOAT3 cp2, float r)
{
	XMVECTOR lp1vec = XMLoadFloat3(&lp1);
	XMVECTOR lp2vec = XMLoadFloat3(&lp2);

	XMVECTOR cp1vec = XMLoadFloat3(&cp1);
	XMVECTOR cp2vec = XMLoadFloat3(&cp2);

	// �����̕����x�N�g��
	XMVECTOR lineDir = XMVectorSubtract(lp2vec, lp1vec);
	XMVECTOR capsuleDir = XMVectorSubtract(cp2vec, cp1vec);

	// �����ƃJ�v�Z���̍ŋߐړ_�����߂�
	float t1, t2;
	XMVECTOR closestLinePoint, closestCapsulePoint;
	ClosestPtSegmentSegment(lp1vec, lineDir, cp1vec, capsuleDir, t1, t2, closestLinePoint, closestCapsulePoint);

	// �ŋߐړ_�Ԃ̋������v�Z
	XMVECTOR distVec = XMVectorSubtract(closestLinePoint, closestCapsulePoint);
	float distSq = XMVectorGetX(XMVector3LengthSq(distVec));

	// �J�v�Z���̔��a���l�����ďՓ˔���
	return distSq <= (r * r);
}




BOOL CollisionSphereSphere(XMFLOAT3 center1, float r1, XMFLOAT3 center2, float r2)
{
	BOOL ans = FALSE;
	XMVECTOR p1 = XMLoadFloat3(&center1);
	XMVECTOR p2 = XMLoadFloat3(&center2);

	XMVECTOR dist = p1 - p2;

	float len;

	XMStoreFloat(&len, XMVector3Length(dist));

	if (len<r1+r2)
	{
		ans = TRUE;
	}

	return ans;
}

BOOL CollisionSphereCapsule(XMFLOAT3 center1, float r1, XMFLOAT3 cp1, XMFLOAT3 cp2, float r2)
{
	BOOL ans = FALSE;

	XMVECTOR p = XMLoadFloat3(&center1);

	XMVECTOR p1 = XMLoadFloat3(&cp1);
	XMVECTOR p2 = XMLoadFloat3(&cp2);

	XMVECTOR p1c = p - p1;
	XMVECTOR p2c = p - p2;
	float p1l;
	float p2l;
	XMStoreFloat(&p1l, XMVector3Length(p1c));
	XMStoreFloat(&p2l, XMVector3Length(p2c));


	//�����̗��[���܂��͌���
	if (p1l < r1+r2 || p2l < r1+r2)
	{
		return TRUE;
	}

	XMVECTOR lv = XMVector3LinePointDistance(p1, p2, p);
	float length;
	XMStoreFloat(&length, lv);
	if (length < r1+r2)
	{
		return TRUE;
	}

	return ans;
}

BOOL CollisionCapsuleCapsule(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2, float r1, XMFLOAT3 cp2_1, XMFLOAT3 cp2_2, float r2)
{
	XMVECTOR p1_1 = XMLoadFloat3(&cp1_1);
	XMVECTOR p1_2 = XMLoadFloat3(&cp1_2);
	XMVECTOR p2_1 = XMLoadFloat3(&cp2_1);
	XMVECTOR p2_2 = XMLoadFloat3(&cp2_2);

	XMVECTOR d1 = XMVectorSubtract(p1_2, p1_1);
	XMVECTOR d2 = XMVectorSubtract(p2_2, p2_1);

	float t1, t2;
	XMVECTOR c1, c2;

	// �ŋߐړ_�����߂�
	ClosestPtSegmentSegment(p1_1, d1, p2_1, d2, t1, t2, c1, c2);

	// �J�v�Z���̔��a���l�����ďՓ˔���
	XMVECTOR distVec = XMVectorSubtract(c1, c2);
	float distSq = XMVectorGetX(XMVector3LengthSq(distVec));
	float radiusSum = r1 + r2;

	return distSq <= (radiusSum * radiusSum);
}

void ClosestPtSegmentSegment(XMVECTOR p1, XMVECTOR d1, XMVECTOR p2, XMVECTOR d2, float& t1, float& t2, XMVECTOR& c1, XMVECTOR& c2)
{
	XMVECTOR r = XMVectorSubtract(p1, p2);
	float a = XMVectorGetX(XMVector3Dot(d1, d1));
	float e = XMVectorGetX(XMVector3Dot(d2, d2));
	float f = XMVectorGetX(XMVector3Dot(d2, r));

	// �ǂ��炩�̐������_�̏ꍇ
	if (a <= FLT_EPSILON && e <= FLT_EPSILON)
	{
		t1 = t2 = 0.0f;
		c1 = p1;
		c2 = p2;
		return;
	}
	if (a <= FLT_EPSILON)
	{
		t1 = 0.0f;
		t2 = f / e;
		t2 = std::clamp(t2, 0.0f, 1.0f);
	}
	else
	{
		float c = XMVectorGetX(XMVector3Dot(d1, r));
		if (e <= FLT_EPSILON)
		{
			t2 = 0.0f;
			t1 = std::clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			float b = XMVectorGetX(XMVector3Dot(d1, d2));
			float denom = a * e - b * b;

			if (denom != 0.0f)
			{
				t1 = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else
			{
				t1 = 0.0f;
			}

			t2 = (b * t1 + f) / e;

			if (t2 < 0.0f)
			{
				t2 = 0.0f;
				t1 = std::clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t2 > 1.0f)
			{
				t2 = 1.0f;
				t1 = std::clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	c1 = XMVectorAdd(p1, XMVectorScale(d1, t1));
	c2 = XMVectorAdd(p2, XMVectorScale(d2, t2));
}



