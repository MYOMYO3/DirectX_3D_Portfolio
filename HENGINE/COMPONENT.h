#pragma once
#include <REF.h>
#include <NAME.h>
#include <DEBUG.h>
#include <MATHS.h>
#include <DATATYPE.h>

#include "UPDATEBASE.h"
#include "ACTOR.h"
// �� ������Ʈ���� �׷���.
// ���� � ������Ʈ�� ������ ����� ���¾־�!
// ���� �ϳ��� ������Ʈ �ȿ� 1���̻� ������ ����.

class Transform;
class Component : public Ref, public UpdateBase, public DataType
{
private:
	friend Actor;

public:
	inline bool IsUpdate() override
	{
		return ((true == m_IsUpdate) && (false == m_IsDeath) && (true == GetActor()->IsUpdate()));
	}
	inline bool IsDeath() override
	{
		return (m_IsDeath || (true == GetActor()->IsDeath()));
	}

	//inline bool Death() override
	//{
	//	m_IsDeath = true;
	//}

private:
	Actor* m_pActor;
	void SetActor(Actor* _Actor) 	{ m_pActor = _Actor; }

public:
	Actor* GetActor()
	{ 
		return m_pActor; 
	}

public:
	Scene* GetScene()
	{
		return GetActor()->GetScene();
	}

private:
	Transform* m_pTrans;

public:
	Transform* GetTransform()
	{
		return m_pTrans;
	}

public:
	virtual void Init() {}
	void ComponentInit();

public:
	void DataCreate() {}

public:
	virtual void SceneChangeStart();

public:
	Component();
	~Component();
};

