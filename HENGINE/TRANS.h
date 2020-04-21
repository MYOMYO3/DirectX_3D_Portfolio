#pragma once
#include "COMPONENT.h"

// ���������� ���� ������ �ô� �༮
// ũ�� ��ġ 

// ���� ���ڸ� �ϴ� ���
// ������ ù�� ��°�� �ϴ� �̹����� ����
// �ִϸ��̼� ����.

// 8�ϵ��� �������� ��´�. 1��.


//matrix Pos;
//matrix Scale;
//matrix Rot;
//matrix World;
//matrix View;
//matrix Proj;
//matrix WV;
//matrix VP;
//matrix WVP;


enum MATTYPE
{
	HMAT_POS,
	HMAT_SCALE,
	HMAT_ROT,
	HMAT_WORLD,
	HMAT_VIEW,
	HMAT_PROJ,
	HMAT_WV,
	HMAT_VP,
	HMAT_WVP,
	HMAT_MAX,
};

class MatrixData
{
public:
	Matrix ARRMAT[HMAT_MAX];
	Vector4 CAMPOS;
	Vector4 RENDERDATA;
	//int LightCount;
	//int ArrLight[64]; 0 3 2

public:
	void CalSubTrans()
	{
		ARRMAT[HMAT_WV] = ARRMAT[HMAT_WORLD] * ARRMAT[HMAT_VIEW];
		ARRMAT[HMAT_VP] = ARRMAT[HMAT_VIEW] * ARRMAT[HMAT_PROJ];
		ARRMAT[HMAT_WVP] = ARRMAT[HMAT_WORLD] * ARRMAT[HMAT_VIEW] * ARRMAT[HMAT_PROJ];
	}

	void AllTP()
	{
		for (size_t i = 0; i < HMAT_MAX; i++)
		{
			ARRMAT[i].TransPose();
		}
	}
};


class Actor;
class Transform : public Component
{
public:
	friend Actor;

public:
	bool m_RotCheck;
	bool m_PosCheck;
	bool m_ScaleCheck;

protected:
	Vector4 m_LocalScale;
	Vector4 m_LocalRot;
	Vector4 m_LocalPos;

	Vector4 m_WScale;
	Vector4 m_WRot;
	Vector4 m_WPos;

	Vector4 m_LQ;
	Vector4 m_WQ;

	Matrix m_ScaleMat;
	Matrix m_ROTX;
	Matrix m_ROTY;
	Matrix m_ROTZ;
	Matrix m_RotMat;
	Matrix m_PosMat;
	
	Matrix m_ParentMatrix;	//���� ��Ʈ����
	Matrix m_WorldMatrix;

	Vector4 m_Scale;
	Vector4 m_Rotation;
	Vector4 m_Position;

	// ���� �ƹ��͵� ��ȭ�� �����
	// �θ� ��ȭ�� �־��ٸ�
	// ���� �������� �Ѵ�.
	bool m_bUpdate;

protected:
	Transform* m_pParentTrans;

	void SetParentTrans(Transform* _pParentTrans)
	{
		m_pParentTrans = _pParentTrans;
	}
	// Local
public:
	Vector4 GetLocalPosition()
	{
		return m_LocalPos;
	}

	void SetLoclaPosition(const Vector4& _Pos)
	{
		m_LocalPos = _Pos;
		m_PosCheck = true;
	}

	void SetLoclaPositionY(const float& _Value)
	{
		m_LocalPos.y = _Value;
		m_PosCheck = true;
	}

	void LocalTranslate(const Vector4& _Pos)
	{
		m_LocalPos += _Pos;
		m_PosCheck = true;
	}

	void LocalTranslateY(const float& _Pos)
	{
		m_LocalPos.y += _Pos;
		m_PosCheck = true;
	}

	//void LocalTranslate3D(const Vector4& _Pos)
	//{
	//	m_LocalPos += _Pos.Vec3;
	//	m_PosCheck = true;
	//}

	Vector4 GetLocalRotation()
	{
		return m_LocalRot;
	}

	Vector4 GetLocalRadRotation()
	{
		return m_LocalRot * Constant::D2R;
	}

	void LocalRotXSum(const float& _Value)
	{
		m_LocalRot.x += _Value;
		m_RotCheck = true;
	}
	void LocalRotYSum(const float& _Value)
	{
		m_LocalRot.y += _Value;
		m_RotCheck = true;
	}
	void LocalRotZSum(const float& _RotZ)
	{
		m_LocalRot.z += _RotZ;
		m_RotCheck = true;
	}

	void SetLocalRotY(const float& _Value)
	{
		m_LocalRot.y = _Value;
		m_RotCheck = true;
	}
	void SetLocalRotX(const float& _Value)
	{
		m_LocalRot.x = _Value;
		m_RotCheck = true;
	}
	void SetLocalRotZ(const float& _RotZ)
	{
		m_LocalRot.z = _RotZ;
		m_RotCheck = true;
	}

	void SetLocalRot(const Vector4& _Rot)
	{
		m_LocalRot = _Rot;
		m_RotCheck = true;
	}

	void LQ(const Vector4& _Q)
	{
		m_LocalRot.QtoR(_Q);
		m_LocalRot *= Constant::R2D;
		m_RotCheck = true;

		//m_LocalRot = _Q;
		//m_RotCheck = true;
	}


	Vector4 GetLocalScale()
	{
		return m_LocalScale;
	}

	float LBigScale()
	{
		float Scale = m_LocalScale.x;

		if (Scale <= m_LocalScale.y)
		{
			Scale = m_LocalScale.y;
		}

		if (Scale <= m_LocalScale.z)
		{
			Scale = m_LocalScale.z;
		}

		return Scale;
	}

	void SetLocalScale(const Vector4& _Scale)
	{
		m_LocalScale = _Scale;
		m_ScaleCheck = true;
	}

	void LocalScaleSum(const float _Scale)
	{
		m_LocalScale += _Scale;
		m_ScaleCheck = true;
	}

	void LocalScaleMul(const float _Scale)
	{
		m_LocalScale *= _Scale;
		m_ScaleCheck = true;
	}

	// ��� ���� ���ߴ�.
	Vector4 LocalLookForward()
	{
		return m_RotMat.ArrVec[2];
	}

	Vector4 LocalLookUp()
	{
		return m_RotMat.ArrVec[1];
	}

	Vector4 LocalLookRight()
	{
		return m_RotMat.ArrVec[0];
	}

	Vector4 LocalLookBack()
	{
		return -m_RotMat.ArrVec[2];
	}

	Vector4 LocalLookDown()
	{
		return -m_RotMat.ArrVec[1];
	}

	Vector4 LocalLookLeft()
	{
		return -m_RotMat.ArrVec[0];
	}


	inline virtual bool UpdateOn()
	{
		UpdateCheck();


		// �ڽ��� ���� �θ��� Ʈ���� ���� ������Ʈ�� �϶�� ���ִ°� ����.

		return m_IsUpdate = true;
	}

	void UpdateCheck();

	inline virtual bool UpdateOff()
	{
		return m_IsUpdate = false;
	}


	// WORLD
public:
	Vector4 GetPosition()
	{
		// ũ����� ����ƴ�.
		return m_Position;
	}

	Vector4 GetPosition2D()
	{
		Vector4 Pos2D;
		Pos2D.x = m_Position.x;
		Pos2D.y = m_Position.y;

		// ũ����� ����ƴ�.
		return Pos2D;
	}

	void SetPostionY(float _PosY)
	{
		Vector4 Pos = GetPosition();
		Pos.y = _PosY;
		SetPosition(Pos);
	}

	void SetPosition(const Vector4& _Pos)
	{
		// �� �󸶷� �ٲ���� �س���.
		// ��Ȯ�ϰ� �׼�ġ�� �ƴҼ� �ִ�.
		// ���� ���� ������ ������ �ִ�.
		//m_WPos = _Pos;
		// �θ� Scale 2, 2, 2
		// �θ� LPos 2, 0, 0
		// ���� Scale 2, 2, 2
		// ���� LPos 3, 0, 0

		// ���� ���� Pos 2 + 6 
		// ���� ���� scale 4, 4, 4

		// ���� ���� Pos 10, 0, 0
		// �θ� LPos 2, 0, 0
		// ������������ 4, 0, 0

		// 10, 0.0f, 0.0f - 2.0f, 0.0f, 0.0f

		if (nullptr == m_pParentTrans)
		{
			m_Position = _Pos;
			m_LocalPos = _Pos;
			m_PosCheck = true;
			return;
		}

		Vector4 PScale = m_pParentTrans->GetLocalScale();
		Vector4 PivotPos = _Pos - m_pParentTrans->GetLocalPosition();

		m_Position = _Pos;
		m_LocalPos = { 
			  PScale.x != 0.0f ? PivotPos.x / PScale.x : PivotPos.x,
			  PScale.y != 0.0f ? PivotPos.y / PScale.y : PivotPos.y,
			  PScale.z != 0.0f ? PivotPos.z / PScale.z : PivotPos.z };// ���������� �ȴ�.
		m_PosCheck = true;
	}

	void WMoveY(float _PosY)
	{
		Vector4 Pos = GetPosition();
		Pos.y += _PosY;
		Pos.w = 1.0F;
		SetPosition(Pos);
	}

	void WMove(const Vector4& _Pos)
	{
		Vector4 Pos = GetPosition();
		Pos += _Pos;
		Pos.w = 1.0F;
		SetPosition(Pos);
	}



	Vector4 GetRotation()
	{
		return m_Rotation;
	}

	Vector4 GetRadRotation()
	{
		return m_Rotation * Constant::D2R;
	}

	void RotationZSum(const float& _RotZ)
	{
		Vector4 WRotCheck = GetRotation();
		WRotCheck.z += _RotZ;
		SetRotation(WRotCheck);
	}

	void RotationYSum(const float& _RotY)
	{
		Vector4 WRotCheck = GetRotation();
		WRotCheck.y += _RotY;
		SetRotation(WRotCheck);
	}

	void WRotZ(const float& _RotZ)
	{

	}

	void QtoWRot(const Vector4& _Q)
	{
		Vector4 Rot;
		Rot.QtoR(_Q);
		SetRotation(Rot * Constant::R2D);
	}


	void SetRotation(const Vector4& _Rot)
	{
		// ���� 20 �� ���Ҵ�.
		// �θ� 20 �� ���Ҿ�
		// lOCALROT��????????????

		// ���� LOCALROTX 20

		// �θ��� LOCALROTX 45
		// ���ο� ������ ������ 135

		if (nullptr == m_pParentTrans)
		{
			m_LocalRot = _Rot;
			m_RotCheck = true;
			return;
		}

		
		Vector4 ParentRot = m_pParentTrans->GetLocalRotation();
		m_Rotation = _Rot;
		m_LocalRot = { _Rot.x - ParentRot.x, _Rot.y - ParentRot.y, _Rot.z - ParentRot.z };// ���������� �ȴ�.
		m_RotCheck = true;
	}

	Vector4 GetScale()
	{
		return m_Scale;
	}

	float WBigScale()
	{
		float Scale = m_Scale.x;

		if (Scale <= m_Scale.y)
		{
			Scale = m_Scale.y;
		}

		if (Scale <= m_Scale.z)
		{
			Scale = m_Scale.z;
		}

		return Scale;
	}

	void SetScale(const Vector4& _Scale)
	{
		// �θ� 2, 2
		// ���� 3, 3

		// ���� ����ũ��� ���� 6 6 
		// �׷��� ���� ���� ũ�⸦ 1�� �ٲٷ�.

		// ������ ũ�Ⱑ �󸶵� ����� �ֳ�?

		if (nullptr == m_pParentTrans)
		{
			m_LocalScale = _Scale;
			m_ScaleCheck = true;
			return;
		}

		Vector4 ParentScale = m_pParentTrans->GetLocalScale();
		m_Scale = _Scale;
		m_LocalScale = { 
			ParentScale.x != 0 ? _Scale.x / ParentScale.x : _Scale.x,
			ParentScale.y != 0 ? _Scale.y / ParentScale.y : _Scale.y,
			ParentScale.z != 0 ? _Scale.z / ParentScale.z : _Scale.z
		};// ���������� �ȴ�.
		m_ScaleCheck = true;
	}

	void SumScale(const float _Scale)
	{
		// �θ� ������ �׳� ���÷� �����ϸ� �ǳ�

		if (nullptr == m_pParentTrans)
		{
			LocalScaleSum(_Scale);
		}
		else {
			Vector4 WScaleCheck = GetScale();
			WScaleCheck += _Scale;
			SetScale(WScaleCheck);
		}

	}

	void WScaleMul(const float _Scale)
	{
	}

	// ��� ���� ���ߴ�.
	Vector4 LookForward()
	{
		return m_WorldMatrix.ArrVec[2].ReturnNormalization();
	}

	Vector4 LookBack()
	{
		return -LookForward();
	}

	Vector4 LookUp()
	{
		return m_WorldMatrix.ArrVec[1].ReturnNormalization();
	}

	Vector4 LookRight()
	{
		return m_WorldMatrix.ArrVec[0].ReturnNormalization();
	}

public: // MATRIX
	Matrix GetWorldMatrix()
	{
		return m_WorldMatrix;
	}

	Matrix WMATToSelect(bool _Scale, bool _Rot, bool _Pos)
	{
		Matrix W = Matrix();
		if (_Scale)		{			W *= m_ScaleMat;		}
		if (_Rot)		{			W *= m_RotMat;		}
		if (_Pos)		{			W *= m_PosMat;		}
		return W;
	}

protected:
	virtual void UpdateTransform();
	virtual void UpdateTransToSelect(bool _Scale, bool _Rot, bool _Pos);

public:
	void Init() override;

public:
	void RenderBefore() override;
	void RenderAfter() override;

public:
	Transform();
	~Transform();
};

